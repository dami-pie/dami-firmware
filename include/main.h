#if !defined(__main_h__)
#define __main_h__

#include <Arduino.h>
#include "mqtt.h"
#include "wifi.h"
#include "screen.h"
#include "otp.h"
#include "config.h"
#include "cmd/cmd.h"

TaskHandle_t ui_watcher;
device_configurations_t config;
volatile bool reconnecting = false;
volatile bool connected = false;

void update_qr_code_task(void *);
#endif // __main_h__
