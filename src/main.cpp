#include "main.h"

String cmd_buffer;

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  SPIFFS.begin();
  start_console(&cmd_buffer);
  add_command("wifi", wifi_cli);
}

void loop()
{
}