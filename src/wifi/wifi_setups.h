#if !defined(__wifi_setups_h__)
#define __wifi_setups_h__

#include <Arduino.h>
#include <WiFi.h>
#include "config/config_data.h"

bool await_connection_result(wl_status_t status = WL_CONNECTED, uint32_t timeout = 15000UL);
void setup_network(size_t index);
void setup_wifi(size_t argc = 0, char **argv = NULL);
void show_wifi_setup(size_t argc = 0, char **argv = NULL);
void add_wifi_cmd(size_t argc = 0, char **argv = NULL);

#endif // __wifi_setups_h__
