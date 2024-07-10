#include "main.h"
#include <WiFiClientSecure.h>

WiFiClientSecure client;
QueueHandle_t ui_render_queue = xQueueCreate(255, 1);

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  SPIFFS.begin();
  start_console(NULL);
  add_command("wifi", wifi_cli);
  add_command("config", config_cli);
  xTaskCreate(ui_task, "ui", 4096, NULL, 2, NULL);
}

void loop()
{
}