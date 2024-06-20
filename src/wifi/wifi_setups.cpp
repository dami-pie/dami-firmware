#include "wifi_setups.h"

bool await_connection_result(wl_status_t status, uint32_t timeout)
{
  for (uint32_t i = 0; WiFi.status() != status && i < timeout; i += 100)
    delay(100);

  return WiFi.status() == status;
}

void setup_network(size_t index)
{
  WiFi.disconnect(true);
  delayMicroseconds(100);
  WiFi.mode(WIFI_STA);

  auto &target_wifi = config.wifi.saved_access_points[index % 4];
  String ssid = target_wifi.ssid.toString();
  String password = target_wifi.password.toString();

  if (!ssid.length())
  {
    log_e("Missing wifi ssid, config index: %u", index);
    return;
  }

  log_i("Setting WiFi configuration SSID %s...", ssid.c_str());
  switch (target_wifi.security)
  {
  case WPA_ACESS_POINT:
  case WPA2_ACESS_POINT:
    WiFi.begin(ssid, password);
    break;

  case PEAP_ACESS_POINT:

    WiFi.begin(ssid.c_str(),
               WPA2_AUTH_PEAP,
               target_wifi.id.length() ? target_wifi.id.toString().c_str() : NULL,
               target_wifi.username.length() ? target_wifi.username.toString().c_str() : NULL,
               password.c_str());
    break;

  default:
    log_e("Unsuported wifi connection");
    return;
  }

  if (await_connection_result(WL_CONNECTED))
    log_i("WiFi connected! IP address: %s", WiFi.localIP().toString());
  else
    log_e("Fail on connection attempt for %s", WIFI_SSID);
}

void setup_wifi()
{
  setup_network(config.wifi.main_accesss_point);

  if (WiFi.status() == WL_CONNECTED)
    return;
  else
    for (size_t i = 0; i < 4; i++)
    {
      if (i == config.wifi.main_accesss_point)
        continue;
      else
        setup_network(i);

      if (WiFi.status() == WL_CONNECTED)
      {
        config.wifi.main_accesss_point = i;
        return;
      }
    }
}
