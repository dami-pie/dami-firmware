#include "config_loader.h"

void set_wifi_to_default_settings()
{
  config.wifi.main_accesss_point = 0;
  config.wifi.saved_access_points[0].ssid = WIFI_SSID;
  config.wifi.saved_access_points[0].password = WIFI_PASSWORD;
  config.wifi.saved_access_points[0].security = WPA2_ACESS_POINT;

  config.wifi.saved_access_points[1].ssid = EAP_SSID;
  config.wifi.saved_access_points[1].username = EAP_SSID;
  config.wifi.saved_access_points[1].password = EAP_PASSWORD;
  config.wifi.saved_access_points[1].security = PEAP_ACESS_POINT;
}

void set_ntp_to_dafault_settings()
{
  config.ntp.ntp_server[0] = NTP_SERVER_1;
  config.ntp.ntp_server[1] = NTP_SERVER_2;
}

void load_wifi_config()
{
  File config_file = SPIFFS.open(WIFI_CONF_FILE);
  size_t content_size;
  if (config_file && config_file.available())
  {
    config_file.read((byte *)&config.wifi, sizeof(wifi_settings_t));
  }
  else if (!SPIFFS.exists(WIFI_CONF_FILE))
  {
    config_file = SPIFFS.open(WIFI_CONF_FILE, "w", true);
    set_wifi_to_default_settings();
    config_file.write((byte *)&config.wifi, sizeof(wifi_settings_t));
  }
  else
    log_e("Fail to load configuration. Unable to open %s file", WIFI_CONF_FILE);
  config_file.close();
}

void load_ntp_config()
{
  File config_file = SPIFFS.open(NTP_SERVERS_CONF_FILE);
  size_t content_size;
  if (config_file && config_file.available())
  {
    config_file.read((byte *)&config.ntp, sizeof(ntp_config_t));
  }
  else if (!SPIFFS.exists(NTP_SERVERS_CONF_FILE))
  {
    config_file = SPIFFS.open(NTP_SERVERS_CONF_FILE, "w", true);
    set_ntp_to_dafault_settings();
    config_file.write((byte *)&config.ntp, sizeof(ntp_config_t));
  }
  else
    log_e("Fail to load configuration. Unable to open %s file", NTP_SERVERS_CONF_FILE);
  config_file.close();
}

void load_broker_config()
{
  if (!SPIFFS.exists(BROKER_CONF_FILE))
    return;

  File config_file = SPIFFS.open(BROKER_CONF_FILE);
  size_t content_size;
  if (config_file && config_file.available())
  {
    config_file.read((byte *)&config.broker, sizeof(broker_config_t));
  }
  else
    log_e("Fail to load configuration. Unable to open %s file", BROKER_CONF_FILE);
  config_file.close();
}

void load_config()
{
  SPIFFS.begin();
  config.id = SPIFFS.open(DEVICE_ID_FILE);
  config.secret = SPIFFS.open(OTP_SECRET_FILE);

  load_wifi_config();
  load_ntp_config();
  load_broker_config();
}
