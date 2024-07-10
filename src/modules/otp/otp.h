#pragma once

#include <Arduino.h>
#include "utils.h"
#include "config/config_data.h"

extern String curr_otp;
class OTP
{
public:
  OTP(StaticString<128> *hmacKey);
  OTP(StaticString<128> *hmacKey, int timeStep);
  char *getCode(long timeStamp);
  char *getCodeFromSteps(long steps);

private:
  // uint8_t *_hmacKey;
  // int _keyLength;
  StaticString<128> *_hmacKey;
  int _timeStep;
  uint8_t _byteArray[8];
  uint8_t *_hash;
  int _offset;
  long _truncatedHash;
  char _code[7];
};

extern OTP otp;
