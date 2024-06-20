#include "main.h"

WiFiClientSecure client;
MQTTClient mqtt;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);

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
  setup_screen();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);
  codeUpdate("0000");
  lv_timer_handler();
  load_config();
  setup_wifi();
  setup_mqtt();

  xTaskCreate(update_totp_task, "totp update", 2048, NULL, 1, NULL);
  xTaskCreate(ui_task, "ui", 4096, NULL, 2, &ui_wacher);
  xTaskCreate(update_qr_code, "ui_update_qr_code", 2048, NULL, 1, &ui_wacher);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED && mqtt.connected())
  {
    show_layout(LV_SYMBOL_WIFI "\tConectado", GREEN_COLOR);
    // codeUpdate(
    //     otp.getCode(
    //         mktime(&timeInfo)));
  }
  else
    show_layout(LV_SYMBOL_CLOSE "\tDesconectado", RED_COLOR);

  getUpdate();
  vTaskDelay(500 / portTICK_PERIOD_MS);
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
    client->connect(WiFi.macAddress().c_str());
    client->subscribe(SECURITY_KEY);
  }

  yield();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);

  vTaskDelay(5000);
}

void update_qr_code(void *)
{

  for (;;)
  {
    xSemaphoreTake(totp_mutex, portMAX_DELAY);
    log_i("updating qr code value to %s", curr_otp);
    lv_qrcode_update(ui_QRCodeLogin, curr_otp, 4);
    log_i("update %s!", pdTRUE == xQueueSend(ui_render_queue, curr_otp, portMAX_DELAY) ? "done" : "fail");
    xSemaphoreGive(totp_mutex);
    vTaskDelay(100);
  }

  vTaskDelete(NULL);
}