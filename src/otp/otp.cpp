#include "otp.h"

byte *load_key()
{
  auto key_file = load_file(OTP_SECRET_FILE);
  if (!key_file)
  {
    log_e("[OTP]: Error on load key");
    return NULL;
  }

  byte *key = (byte *)malloc(key_file.available());
  key_file.readBytes((char *)key, sizeof(key));
  log_i("------ SECURE KEY -----");
  log_i("%s", key);
  log_i("-----------------------");

  return key;
}
byte *key = load_key();

TOTP otp(key, sizeof(key), 60);
