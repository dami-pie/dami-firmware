#pragma once

#include <Arduino.h>

void setup_lock();
void open_lock(uint32_t);
void alert_open();
void alert_close();
void alert_denied();
void alert_allowed();
