#include "config_loader.h"

void set_wifi_to_default()
{
  config.wifi.main_ap = 0;
  config.wifi.saved_ap[0].ssid = WIFI_SSID;
  config.wifi.saved_ap[0].password = WIFI_PASSWORD;
  config.wifi.saved_ap[0].security = WPA2_ACCESS_POINT;

  config.wifi.saved_ap[1].ssid = EAP_SSID;
  config.wifi.saved_ap[1].username = EAP_PASSWORD;
  config.wifi.saved_ap[1].password = EAP_PASSWORD;
  config.wifi.saved_ap[1].security = PEAP_ACCESS_POINT;
}

void set_ntp_to_default()
{
  config.ntp.servers[0] = NTP_SERVER_1;
  config.ntp.servers[1] = NTP_SERVER_2;
}

void save_configuration(const char *path, void *config, size_t size)
{
  if (config == NULL)
  {
    log_e("Impossible to save configuration! configuration pointer is null.");
    return;
  }

  size_t content_size;
  auto config_file = SPIFFS.open(path, "w", true);

  if (config_file.write((byte *)config, size) != size)
    log_e("Fail on save %s", path);

  if (config_file)
    config_file.close();
}

void load_configuration(const char *path, void *config, size_t size, std::function<void()> to_default = NULL)
{
  if (config == NULL)
  {
    log_e("Impossible to load configuration! configuration pointer is null.");
    return;
  }

  File config_file = SPIFFS.open(path);
  size_t content_size;

  if (config_file && config_file.available())
  {
    log_i("Loading configuration file %s...", path);
    auto total_read = config_file.read((byte *)config, size);
    log_d("%u bytes readd from %s", total_read, path);
  }
  else if (to_default != NULL && !SPIFFS.exists(path))
  {
    log_w("Unable to open %s, file dons't exists. Saving default values...", path);
    to_default();
    config_file = SPIFFS.open(path, "w", true);
    config_file.write((byte *)config, size);
  }
  else
    log_e("Fail to load configuration. Unable to open %s file", path);

  if (config_file)
  {
    log_d("Closing file");
    config_file.close();
  }
}

void load_config()
{
  SPIFFS.begin();
  log_i("Loading system configs");

  config.id = SPIFFS.open(DEVICE_ID_FILE);
  log_i("ID loaded %s", config.id.toString().c_str());

  config.secret = SPIFFS.open(OTP_SECRET_FILE);
  log_d("Secret loaded %s", config.secret.toString().c_str());

  // load wifi configurations from file
  load_configuration(WIFI_CONF_FILE, &config.wifi, sizeof(wifi_settings_t), set_wifi_to_default);

  // load ntp configurations from file
  load_configuration(NTP_SERVERS_CONF_FILE, &(config.ntp), sizeof(ntp_config_t), set_ntp_to_default);

  // load broker configurations from file
  load_configuration(BROKER_CONF_FILE, &(config.broker), sizeof(broker_config_t), []() {});
}

void config_cli(size_t argc, char **args)
{
  if (argc < 2)
    return;
  String arg_1 = args[1];
  if (arg_1 == "load" && argc > 2)
  {
    String arg_2 = args[2];

    if (arg_2 == "wifi")
      load_configuration(WIFI_CONF_FILE, &config.wifi, sizeof(wifi_settings_t), set_wifi_to_default);
    else if (arg_2 == "ntp")
      load_configuration(NTP_SERVERS_CONF_FILE, &(config.ntp), sizeof(ntp_config_t), set_ntp_to_default);
  }
  else if (arg_1 == "save")
  {
    save_configuration(WIFI_CONF_FILE, &config.wifi, sizeof(wifi_settings_t));
    save_configuration(NTP_SERVERS_CONF_FILE, &(config.ntp), sizeof(ntp_config_t));
  }
}
