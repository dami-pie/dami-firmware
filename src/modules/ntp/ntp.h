#pragma once

#include <Arduino.h>
#include "config/config_data.h"

bool sync_ntp(uint32_t timeout = 5000);

tm ntp_time(uint32_t timeout = 1000);