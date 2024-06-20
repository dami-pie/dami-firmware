#if !defined(__config_loader_h__)
#define __config_loader_h__
#include "config_data.h"

void set_wifi_to_default_settings();
void set_ntp_to_dafault_settings();

void load_wifi_config();
void load_ntp_config();
void load_broker_config();
void load_config();

#endif // __config_loader_h__
