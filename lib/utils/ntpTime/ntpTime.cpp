#include "ntpTime.h"

struct tm timeInfo;

std::tuple<String, String> timeUpdate(uint32_t timeout)
{
    char time[6];
    char date[11];
    if (getLocalTime(&timeInfo, timeout))
    {
        strftime(time, sizeof(time), "%R", &timeInfo);
        strftime(date, sizeof(date), "%d/%m/%Y", &timeInfo);
        return std::make_tuple(time, date);
    }
    else
        return std::make_tuple("--:--", "00/00/0000");
}
