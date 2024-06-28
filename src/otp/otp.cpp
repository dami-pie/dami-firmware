#include "otp.h"

char curr_otp[4];
SemaphoreHandle_t totp_mutex;

TOTP otp(reinterpret_cast<uint8_t *>(config.secret.getBuffer()), 128, 15);

bool compare(char *str)
{
  if (sizeof(str) != 4)
    return false;

  for (size_t i = 0; i < 4; i++)
    if (str[i] != curr_otp[i])
      return false;

  return true;
}

void update_totp_task(void *)
{
  char new_otp[4];
  totp_mutex = xSemaphoreCreateMutex();
  xSemaphoreTake(totp_mutex, portMAX_DELAY);
  for (;;)
  {
    memcpy(new_otp, otp.getCode(mktime(&timeInfo)), 4);
    if (!compare(new_otp))
    {
      log_i("OTP changed...");
      strcpy(curr_otp, new_otp);
      log_i("giving mutex...");
      xSemaphoreGive(totp_mutex);
      delayMicroseconds(10);
      log_i("taking mutex...");
      log_i("mutex %s!", xSemaphoreTake(totp_mutex, portMAX_DELAY) == pdTRUE ? "taken" : "timeout");
    }
    else
      vTaskDelay(100);
  }

  vTaskDelete(NULL);
  while (true)
    ;
}
