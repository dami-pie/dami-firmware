#if !defined(__main_h__)
#define __main_h__

#include <Arduino.h>
#include "mqtt.h"
#include "wifi.h"
#include "screen.h"
#include "otp.h"
#include "config.h"

TaskHandle_t ui_wacher;
device_configurations_t config;
volatile bool reconnecting = false;
volatile bool connected = false;

void update_qr_code(void *);
#endif // __main_h__
