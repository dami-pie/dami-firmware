
#ifndef __otp_h__
#define __otp_h__

#include "TOTP.h"
#include "utils.h"
#include "config/config_data.h"

void update_totp_task(void *);

extern TOTP otp;
extern char curr_otp[4];
extern SemaphoreHandle_t totp_mutex;
#endif
