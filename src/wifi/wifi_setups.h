#if !defined(__wifi_setups_h__)
#define __wifi_setups_h__

#include <Arduino.h>
#include <WiFi.h>

bool await_connection_result(wl_status_t status = WL_CONNECTED, uint32_t timeout = 15000UL);

void setup_eap_network();

void setup_default_network();
#endif // __wifi_setups_h__
