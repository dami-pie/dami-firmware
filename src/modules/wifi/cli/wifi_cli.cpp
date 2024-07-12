#pragma once

#include "wifi_cli.h"
#include "hash.h"

void wifi_cli_help()
{
}

enum wifi_cli_act_t
{
  SAVE_WIFI_SETTINGS,
  LOAD_WIFI_SETTINGS,
  RESET_WIFI_SETTINGS,
  CONNECT_WIFI_NETWORK,
  SET_WIFI_NETWORK,
  GET_WIFI_NETWORK,
};

wifi_cli_act_t parse_cli_act(String action)
{
  if (action == "add" || action == "set")
    return SET_WIFI_NETWORK;

  if (action == "show" || action == "get")
    return GET_WIFI_NETWORK;

  if (action == "connect")
    return CONNECT_WIFI_NETWORK;

  if (action == "reset")
    return RESET_WIFI_SETTINGS;

  if (action == "save")
    return SAVE_WIFI_SETTINGS;

  if (action == "load")
    return LOAD_WIFI_SETTINGS;
}

void save_wifi_settings()
{
  auto file = SPIFFS.open(WIFI_CONF_FILE, "w", true);
  if (file)
    file.write((byte *)&config.wifi, sizeof(wifi_settings_t));
  else
    log_e("Fail on save wifi configuration, unable to open config file");
  file.close();
}

void load_wifi_settings()
{
  auto file = SPIFFS.open(WIFI_CONF_FILE);
  if (file)
    file.read((byte *)&config.wifi, sizeof(wifi_settings_t));
  else
    log_e("Fail on save wifi configuration, unable to open config file");
  file.close();
}

void get_wifi_config(int index)
{
  if (index < 0 || index >= 4)
    return (void)log_e("Invalid wifi slot");

  Serial.printf("ID[%u]: %s\n", index, config.wifi.saved_ap[index].id.toString().c_str());
  Serial.printf("SSID[%u]: %s\n", index, config.wifi.saved_ap[index].ssid.toString().c_str());
  Serial.printf("USERNAME[%u]: %s\n", index, config.wifi.saved_ap[index].username.toString().c_str());
  Serial.printf("PASSWORD[%u]: %s\n", index, config.wifi.saved_ap[index].password.toString().c_str());
  Serial.printf("SECURITY[%u]: ", index);
  switch (config.wifi.saved_ap[index].security)
  {
  case PEAP_ACCESS_POINT:
    Serial.println("PEAP");
    break;
  case WPA2_ACCESS_POINT:
    Serial.println("WPA2");
    break;
  case WPA_ACCESS_POINT:
    Serial.println("WPA");
    break;

  default:
    Serial.println("undefined");
    break;
  }
}

void connect_wifi_network(int index)
{
  setup_network(index);

  if (WiFi.status() == WL_CONNECTED)
    return;
  else
    for (size_t i = 0; i < 4; i++)
    {
      if (i == index)
        continue;
      else
        setup_network(i);

      if (WiFi.status() == WL_CONNECTED)
      {
        config.wifi.main_ap = i;
        return;
      }
    }
}

void show_wifi_setup()
{
  Serial.print("WIFI STATUS: ");
  switch (WiFi.status())
  {
  case WL_IDLE_STATUS:
    Serial.println("IDLE STATUS");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println("NO SSID AVAIL");
    break;
  case WL_SCAN_COMPLETED:
    Serial.println("SCAN COMPLETED");
    break;
  case WL_CONNECTED:
    Serial.println("CONNECTED");
    break;
  case WL_CONNECT_FAILED:
    Serial.println("CONNECT FAILED");
    break;
  case WL_CONNECTION_LOST:
    Serial.println("CONNECTION LOST");
    break;
  case WL_DISCONNECTED:
    Serial.println("DISCONNECTED");
    break;
  default:
    Serial.println("unknow");
  }
  Serial.printf("DEFAULT AP: %u\n", config.wifi.main_ap);
  for (size_t i = 0; i < 4; i++)
  {
    Serial.printf("ID[%u]: %s\n", i, config.wifi.saved_ap[i].id.toString().c_str());
    Serial.printf("SSID[%u]: %s\n", i, config.wifi.saved_ap[i].ssid.toString().c_str());
    Serial.printf("USERNAME[%u]: %s\n", i, config.wifi.saved_ap[i].username.toString().c_str());
    Serial.printf("PASSWORD[%u]: %s\n", i, config.wifi.saved_ap[i].password.toString().c_str());
    Serial.printf("SECURITY[%u]: ", i);
    switch (config.wifi.saved_ap[i].security)
    {
    case PEAP_ACCESS_POINT:
      Serial.println("PEAP");
      break;
    case WPA2_ACCESS_POINT:
      Serial.println("WPA2");
      break;
    case WPA_ACCESS_POINT:
      Serial.println("WPA");
      break;

    default:
      Serial.println("undefined");
      break;
    }
    for (size_t i = 0; i < 64; i++)
      Serial.print("-");

    Serial.println();
  }
}

void wifi_cli(size_t argc, char **argv)
{
  if (argc <= 1)
    return;

  int index = (argc >= 3) ? String(argv[2]).toInt() : -1;

  switch (parse_cli_act(argv[1]))
  {
  case CONNECT_WIFI_NETWORK:
    if (index > 0)
      connect_wifi_network(index);
    else
      connect_wifi_network();
    return;

  case SAVE_WIFI_SETTINGS:
    return save_wifi_settings();

  case LOAD_WIFI_SETTINGS:
    return load_wifi_settings();

  case GET_WIFI_NETWORK:
    if (argc <= 2)
    {
      get_wifi_config(0);
      Serial.println("--------------------------------------");
      get_wifi_config(1);
      Serial.println("--------------------------------------");
      get_wifi_config(2);
      Serial.println("--------------------------------------");
      get_wifi_config(3);
      return;
    }
    else
      return get_wifi_config(index);

  case SET_WIFI_NETWORK:
    if (argc < 5)
    {
      log_e("Invalid arguments");
      return;
    }
    else if (index < 0 || index >= 4)
      return (void)log_e("Invalid wifi slot");

    config.wifi.saved_ap[index].ssid = argv[3];
    config.wifi.saved_ap[index].password = argv[4];
    if (argc >= 6)
    {
      config.wifi.saved_ap[index].security_b = String(argv[5]).toInt();
      if (config.wifi.saved_ap[index].security == PEAP_ACCESS_POINT)
      {
        if (argc >= 7)
          config.wifi.saved_ap[index].username = argv[6];
        if (argc >= 8)
          config.wifi.saved_ap[index].username = argv[7];
      }
    }
    else
      config.wifi.saved_ap[index].security = WPA2_ACCESS_POINT;

    return;

  default:
    wifi_cli_help();
  }
}