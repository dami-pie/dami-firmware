#include "wifi_setups.h"

bool await_connection_result(wl_status_t status = WL_CONNECTED, uint32_t timeout = 15000UL)
{
  for (uint32_t i = 0; WiFi.status() != status && i < timeout; i += 100)
    delay(100);

  return WiFi.status() != WL_CONNECTED;
}

void setup_eap_network()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  // This part of the code is taken from the oficial wpa2_enterprise example from esp-idf
  log_i("Setting WiFi configuration SSID %s...", EAP_SSID);

  WiFi.begin(EAP_SSID,
             WPA2_AUTH_PEAP,
             EAP_ID,
             EAP_USERNAME,
             EAP_PASSWORD);

  if (await_connection_result(WL_CONNECTED))
    log_i("WiFi connected! IP address: %s", WiFi.localIP().toString());
  else
    log_e("Fail on connection attempt for %s", EAP_SSID);
}

void setup_default_network()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  // This part of the code is taken from the oficial wpa2_enterprise example from esp-idf
  log_i("Setting WiFi configuration SSID %s...", WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (await_connection_result(WL_CONNECTED))
    log_i("WiFi connected! IP address: %s", WiFi.localIP().toString());
  else
    log_e("Fail on connection attempt for %s", WIFI_SSID);
}