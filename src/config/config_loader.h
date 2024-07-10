#if !defined(__config_loader_h__)
#define __config_loader_h__
#include "config_data.h"

void set_wifi_to_default();
void set_ntp_to_default();

void load_config();
void config_cli(size_t, char **);

#endif // __config_loader_h__
