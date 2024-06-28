#if !defined(__config_data_h__)
#define __config_data_h__

#include <static_string.h>
#include <WiFi.h>

typedef enum : uint8_t
{
  PEAP_ACCESS_POINT = 1,
  WPA2_ACCESS_POINT = 2,
  WPA_ACCESS_POINT = 3
} access_point_security_t;

typedef struct
{

  union
  {
    uint8_t security_b;
    access_point_security_t security;
  };
  StaticString<32> id;
  StaticString<32> ssid;
  StaticString<32> username;
  StaticString<32> password;
} wifi_access_point_save_t;

typedef struct
{
  uint8_t main_ap;
  wifi_access_point_save_t saved_ap[4];
} wifi_settings_t;

typedef struct
{
  uint16_t offset;
  StaticString<16> servers[3];
} ntp_config_t;

typedef struct
{
  bool use_auth;
  StaticString<16> login;
  StaticString<16> password;
} broker_auth_t;

typedef struct
{
  broker_auth_t auth;
} broker_config_t;

typedef struct
{
  StaticString<64> id;
  StaticString<128> secret;
  wifi_settings_t wifi;
  ntp_config_t ntp;
  broker_config_t broker;
} device_configurations_t;

extern device_configurations_t config;

#endif // __config_data_h__
