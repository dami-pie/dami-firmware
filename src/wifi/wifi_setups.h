#if !defined(__wifi_setups_h__)
#define __wifi_setups_h__

#include <Arduino.h>
#include "esp_wpa2.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include <WiFi.h>

void setup_eap_network()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  // This part of the code is taken from the oficial wpa2_enterprise example from esp-idf
  log_i("Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);

  WiFi.begin(EAP_SSID,
             WPA2_AUTH_PEAP,
             EAP_ID,
             EAP_ID,
             EAP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

#endif // __wifi_setups_h__
