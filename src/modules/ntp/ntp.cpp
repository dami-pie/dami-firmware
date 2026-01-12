#include "ntp.h"
tm timeInfo;
static uint32_t last_ntp_read;
static uint32_t last_ntp_sync;

bool sync_ntp(uint32_t timeout)
{
  configTime(config.ntp.offset, 0,
             config.ntp.servers[0].getBuffer(),
             config.ntp.servers[1].length() ? config.ntp.servers[1].getBuffer() : NULL,
             config.ntp.servers[2].length() ? config.ntp.servers[2].getBuffer() : NULL);

  bool result = getLocalTime(&timeInfo, timeout);
  if (result)
  {
    last_ntp_read = millis();
    last_ntp_sync = millis();
  }
  return result;
}

tm ntp_time(uint32_t timeout)
{
  if (abs((long)(millis() - last_ntp_sync) >= 36e6))
    sync_ntp(timeout);
  else if (abs((long)(millis() - last_ntp_read) >= 999) && getLocalTime(&timeInfo, timeout))
    last_ntp_read = millis();
  return timeInfo;
};