#include "main.h"

WiFiClientSecure client;
MQTTClient mqtt;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);
String cmd_buffer;

void ui_task(void *args)
{
  for (uint32_t max_delay = 0;; max_delay = lv_timer_handler())
    xQueueReceive(ui_render_queue, &max_delay, max_delay > 1000 ? 1000 : max_delay);

  log_w("ui_task terminated!");
  vTaskDelete(NULL);
}

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  for (int t = 0; t < 2000 && !Serial; t++)
    delay(10);
  setup_screen();
  lv_label_set_text(ui_TimeLabel, "--:--");
  lv_label_set_text(ui_DateLabel, "00/00/0000");
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BLUE_COLOR);
  codeUpdate("0000");
  lv_timer_handler();
  load_config();
  setup_wifi();
  configTime(-10800, 0,
             config.ntp.servers[0].c_str(true),
             config.ntp.servers[1].c_str(true),
             config.ntp.servers[2].c_str(true));

  if (!getLocalTime(&timeInfo))
    log_e("fail on load time! last unix stamp %d", mktime(&timeInfo));
  else
  {
    char time[6];
    char date[11];
    strftime(time, sizeof(time), "%R", &timeInfo);
    lv_label_set_text(ui_TimeLabel, time);
    strftime(date, sizeof(date), "%d/%m/%Y", &timeInfo);
    lv_label_set_text(ui_DateLabel, date);
    lv_timer_handler();
  }

  //   // setup_mqtt();
  xTaskCreate(ui_task, "ui", 4096, NULL, 2, &ui_watcher);
  xTaskCreate(update_totp_task, "totp update", 2048, NULL, 1, NULL);
  xTaskCreate(update_qr_code_task, "ui_update_qr_code", 2048, NULL, 1, NULL);
  cmd_start(&cmd_buffer);

  add_cmd("get wifi", show_wifi_setup);

  add_cmd("wifi connect", setup_wifi);

  add_cmd("wifi restore", [](size_t, char **)
          {
            set_wifi_to_default();
            show_wifi_setup(); });

  add_cmd("wifi save", [](size_t, char **)
          {
              auto file = SPIFFS.open(WIFI_CONF_FILE, "w", true);
              file.write((byte *)&config.wifi, sizeof(wifi_settings_t));
              file.close(); });

  add_cmd("set wifi", add_wifi_cmd);
}

void loop()
{
  // if (WiFi.status() != WL_CONNECTED)
  //   show_layout(LV_SYMBOL_CLOSE "\tDesconectado", RED_COLOR);
  // else
  // {
  //   switch (mqtt.status())
  //   {
  //   case MQ_CONNECTED:
  //     show_layout(LV_SYMBOL_WIFI "\tConectado", GREEN_COLOR);
  //     break;

  //   case MQ_RECONNECTING:
  //     show_layout(LV_SYMBOL_REFRESH "\tReconectando", BLUE_COLOR);
  //     break;

  //   default:
  //     show_layout(LV_SYMBOL_CLOSE "\tDesconectado", RED_COLOR);
  //     break;
  //   }
  // }

  // getUpdate();
  // codeUpdate((CONFIG_BASE_CLIENT_URL "?q=" + config.id.toString() + "&oq=") + otp.getCode(mktime(&timeInfo)));
  // vTaskDelay(500 / portTICK_PERIOD_MS);
}

void message_callback(char *topic, uint8_t *data, size_t size)
{
  char str[size];
  memcpy(str, (const char *)data, size);
  log_i("%s", str);
  lv_qrcode_update(ui_QRCodeLogin, data, size);
  xQueueSend(ui_render_queue, str, portMAX_DELAY);
  ;
}

void reconnect_callback(MQTTClient *client)
{
  if (client)
  {
    client->connect(config.id.toString().c_str());
    client->subscribe(SECURITY_KEY);
  }

  yield();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);
  vTaskDelay(5000);
}

void update_qr_code_task(void *)
{

  for (;;)
  {
    xSemaphoreTake(totp_mutex, portMAX_DELAY);
    log_i("updating qr code value to %s", curr_otp);
    codeUpdate((CONFIG_BASE_CLIENT_URL "?q=" + config.id.toString() + "&oq=") + curr_otp);
    log_i("update %s!", pdTRUE == xQueueSend(ui_render_queue, curr_otp, portMAX_DELAY) ? "done" : "fail");
    xSemaphoreGive(totp_mutex);
    vTaskDelay(100);
  }

  vTaskDelete(NULL);
}