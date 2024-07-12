#include "mqtt_cli.h"
#include "config/config_data.h"
#include <SPIFFS.h>

void mqtt_cli(size_t argc, char **argv)
{
  String arg_1 = argc < 2 ? "show" : argv[1];
  if (arg_1 == "show")
  {
    Serial.printf("broker.port: %u \n\r", config.broker.port);
    Serial.printf("broker.server: %s \n\r", (config.broker.server.toString() + '\0').c_str());
  }
  else if (arg_1 == "set" && argc > 3)
  {
    String arg_2 = argv[2];
    String arg_3 = argv[3];
    if (arg_2 == "broker.port")
      config.broker.port = arg_3.toInt();
    else if (arg_2 == "broker.server")
      config.broker.server = (arg_3 + '\0');
    else
      Serial.println("Invalid mqtt field!");
  }
  else if (arg_1 == "save")
  {
    auto file = SPIFFS.open(BROKER_CONF_FILE, "w", true);

    if (!file)
      return (void)log_e("Fail on save configuration, unable to open %s", BROKER_CONF_FILE);

    if (file.write((byte *)&config.broker, sizeof(broker_config_t)) == sizeof(broker_config_t))
      Serial.println("Successful saved");
    else
      Serial.println("Fail on save");
  }
  else if (arg_1 == "pause")
    mqtt.pause();
  else if (arg_1 == "resume")
    mqtt.resume();
}