#include "main.h"
#include <WiFiClientSecure.h>

String curr_otp;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);
WiFiClientSecure client;
MQTTClient mqtt;

void setup(void)
{
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  SPIFFS.begin();
  load_config();
  // start_console(NULL);
  // add_command("wifi", wifi_cli);
  // add_command("config", config_cli);
  // add_command("ntp", ntp_cli);
  // add_command("mqtt", mqtt_cli);
  connect_wifi_network();
  sync_ntp();
  xTaskCreate(ui_task, "ui", 4096, &curr_otp, 2, NULL);
  delay(1000);
  setup_mqtt(&mqtt);
  setup_lock();
}

void loop()
{
  auto tm = ntp_time(true);
  auto unix_time = mktime(&tm);
  curr_otp = config.id.toString() + "/" + otp.getCode(mktime(&tm));
  // log_v("New otp url (%u): %s", unix_time, curr_otp.c_str());
  delay(1000);
}

void mqtt_reconnect_callback(MQTTClient *client)
{
  yield();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BLACK_COLOR, WHITE_COLOR);
  if (client && client->connect(config.id.getBuffer(), "dami", "517402") && client->subscribe(config.id.getBuffer()))
    show_layout(LV_SYMBOL_WIFI "\tConectado", GREEN_COLOR, BLACK_COLOR);
  else
    show_layout(LV_SYMBOL_CLOSE "\tDesconectado", RED_COLOR, WHITE_COLOR);

  vTaskDelay(5000);
}

void mqtt_message_callback(char *topic, uint8_t *data, size_t size)
{
  String message = (char *)data;
  message = message.substring(0, size);
  log_i("new message: [%s]: %s", topic, message.c_str());

  if (config.id != topic)
    return (void)log_i("Wrong topic");

  if (curr_otp.endsWith(message))
  {
    log_i("Access authorized");
    open_lock(5000UL);
  }
  else
    log_i("Access unauthorized");
}