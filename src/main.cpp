#include "main.h"
#include <WiFiClientSecure.h>

String curr_otp;
WiFiClientSecure client;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);

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
  connect_wifi_network();
  sync_ntp();
  xTaskCreate(ui_task, "ui", 4096, &curr_otp, 2, NULL);
}

void loop()
{
  auto tm = ntp_time(true);
  curr_otp = (CONFIG_BASE_CLIENT_URL "?q=" + config.id.toString() + "&oq=") + otp.getCode(mktime(&tm));
  delay(1000);
}