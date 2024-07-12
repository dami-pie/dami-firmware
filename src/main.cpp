#include "main.h"
#include <WiFiClientSecure.h>

String curr_otp;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);
WiFiClient client;
MQTTClient mqtt;

void setup(void)
{
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  SPIFFS.begin();
  load_config();
  start_console(NULL);
  add_command("wifi", wifi_cli);
  add_command("config", config_cli);
  add_command("ntp", ntp_cli);
  add_command("mqtt", mqtt_cli);
  connect_wifi_network();
  sync_ntp();
  xTaskCreate(ui_task, "ui", 4096, &curr_otp, 2, NULL);
  delay(1000);
  setup_mqtt(&mqtt);
}

void loop()
{
  auto tm = ntp_time(true);
  curr_otp = (CONFIG_BASE_CLIENT_URL "?q=" + config.id.toString() + "&oq=") + otp.getCode(mktime(&tm));
  delay(1000);
}

void mqtt_reconnect_callback(MQTTClient *client)
{
  yield();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);
  if (client && client->connect(config.id.toString().c_str()) && client->subscribe(SECURITY_KEY))
    show_layout(LV_SYMBOL_WIFI "\tConectado", GREEN_COLOR);

  vTaskDelay(5000);
}

void mqtt_message_callback(char *topic, uint8_t *data, size_t size)
{
  char str[size];
  memcpy(str, (const char *)data, size);
  log_i("%s", str);
  lv_qrcode_update(ui_QRCodeLogin, data, size);
  xQueueSend(ui_render_queue, str, portMAX_DELAY);
  ;
}