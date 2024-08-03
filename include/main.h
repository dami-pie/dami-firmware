#if !defined(__main_h__)
#define __main_h__

#include <Arduino.h>
#include "config/config_loader.h"
#include "modules/wifi/cli/wifi_cli.h"
#include "modules/mqtt/cli/mqtt_cli.h"
#include "modules/ntp/cli/ntp_cli.h"
#include "modules/screen/screen.h"
#include "modules/mqtt/mqtt.h"
#include "modules/lock/lock.h"
#include "modules/otp/otp.h"
#include "modules/ntp/ntp.h"
#include "console.h"

device_configurations_t config;

#endif // __main_h__
