#include "ntp_cli.h"

void ntp_cli(size_t argc, char **argv)
{

  String arg_1 = argc < 2 ? "show" : argv[1];
  if (arg_1 == "show")
  {
    Serial.printf("Time offset: %d seconds \n\r", config.ntp.offset);
    Serial.printf("NTP server 1: %s \n\r", (config.ntp.servers[0].toString() + '\0').c_str());
    Serial.printf("NTP server 2: %s \n\r", (config.ntp.servers[1].toString() + '\0').c_str());
    Serial.printf("NTP server 3: %s \n\r", (config.ntp.servers[2].toString() + '\0').c_str());
  }
  else if (arg_1 == "set" && argc > 3)
  {
    String arg_2 = argv[2];
    String arg_3 = argv[3];
    if (arg_2 == "offset")
    {
      config.ntp.offset = arg_3.toInt();
      return;
    }
    else if (arg_2 == "server_1")
      config.ntp.servers[0] = (arg_3 + '\0');
    else if (arg_2 == "server_2")
      config.ntp.servers[1] = (arg_3 + '\0');
    else if (arg_2 == "server_3")
      config.ntp.servers[2] = (arg_3 + '\0');
    else
    {
      Serial.println("Invalid ntp field");
      return;
    }
  }
}