#if !defined(__main_h__)
#define __main_h__

#include <Arduino.h>
#include "mqtt.h"
#include "screen.h"
#include "otp.h"

TaskHandle_t ui_wacher;
volatile bool reconnecting = false;
volatile bool connected = false;

#endif // __main_h__
