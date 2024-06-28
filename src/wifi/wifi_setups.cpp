#include "wifi_setups.h"
#include "cmd/cmd.h"

bool await_connection_result(wl_status_t status, uint32_t timeout)
{
  for (uint32_t i = 0; WiFi.status() != status && i < timeout; i += 100)
    delay(100);

  return WiFi.status() == status;
}

void setup_network(size_t index)
{
  WiFi.disconnect(true);
  delayMicroseconds(100);
  WiFi.mode(WIFI_STA);

  auto &target_wifi = config.wifi.saved_ap[index % 4];
  String ssid = target_wifi.ssid.toString();
  String password = target_wifi.password.toString();

  if (!ssid.length())
  {
    log_e("Missing wifi ssid, config index: %u", index);
    return;
  }

  log_i("Setting WiFi configuration SSID %s...", ssid.c_str());
  switch (target_wifi.security)
  {
  case WPA_ACCESS_POINT:
  case WPA2_ACCESS_POINT:
    WiFi.begin(ssid, password);
    break;

  case PEAP_ACCESS_POINT:
    // String id = target_wifi.id.toString(), username = target_wifi.username.toString();
    // WiFi.begin(ssid.c_str(),
    //            WPA2_AUTH_PEAP,
    //            id.length() ? id.c_str() : NULL,
    //            username.length() ? username.c_str() : NULL,
    //            password.c_str());
    break;

  default:
    log_e("Unsupported wifi connection");
    return;
  }

  if (await_connection_result(WL_CONNECTED))
    log_i("WiFi connected! IP address: %s", WiFi.localIP().toString());
  else
    log_e("Fail on connection attempt for %s", WIFI_SSID);
}

void setup_wifi(size_t argc, char **argv)
{
  setup_network(config.wifi.main_ap);

  if (WiFi.status() == WL_CONNECTED)
    return;
  else
    for (size_t i = 0; i < 4; i++)
    {
      if (i == config.wifi.main_ap)
        continue;
      else
        setup_network(i);

      if (WiFi.status() == WL_CONNECTED)
      {
        config.wifi.main_ap = i;
        return;
      }
    }
}

void show_wifi_setup(size_t argc, char **argv)
{
  Serial.print("WIFI STATUS: ");
  switch (WiFi.status())
  {
  case WL_IDLE_STATUS:
    Serial.println("IDLE STATUS");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println("NO SSID AVAIL");
    break;
  case WL_SCAN_COMPLETED:
    Serial.println("SCAN COMPLETED");
    break;
  case WL_CONNECTED:
    Serial.println("CONNECTED");
    break;
  case WL_CONNECT_FAILED:
    Serial.println("CONNECT FAILED");
    break;
  case WL_CONNECTION_LOST:
    Serial.println("CONNECTION LOST");
    break;
  case WL_DISCONNECTED:
    Serial.println("DISCONNECTED");
    break;
  default:
    Serial.println("unknow");
  }
  Serial.printf("DEFAULT AP: %u\n", config.wifi.main_ap);
  for (size_t i = 0; i < 4; i++)
  {
    Serial.printf("ID[%u]: %s\n", i, config.wifi.saved_ap[i].id.toString().c_str());
    Serial.printf("SSID[%u]: %s\n", i, config.wifi.saved_ap[i].ssid.toString().c_str());
    Serial.printf("USERNAME[%u]: %s\n", i, config.wifi.saved_ap[i].username.toString().c_str());
    Serial.printf("PASSWORD[%u]: %s\n", i, config.wifi.saved_ap[i].password.toString().c_str());
    Serial.printf("SECURITY[%u]: ", i);
    switch (config.wifi.saved_ap[i].security)
    {
    case PEAP_ACCESS_POINT:
      Serial.println("PEAP");
      break;
    case WPA2_ACCESS_POINT:
      Serial.println("WPA2");
      break;
    case WPA_ACCESS_POINT:
      Serial.println("WPA");
      break;

    default:
      Serial.println("undefined");
      break;
    }
    for (size_t i = 0; i < 64; i++)
      Serial.print("-");

    Serial.println();
  }
}

void wifi_cmd(size_t argc, char **argv)
{
  struct
  {
    bool add : 1;
    bool rm : 1;
    bool show : 1;
    bool connect : 1;
    bool reset : 1;
    bool pad : 2;
  } opt;
}

void add_wifi_cmd(size_t argc, char **argv)
{
  String buf;
  Serial.print("Index: ");
  cmd_input(buf);
  auto &net = config.wifi.saved_ap[buf.toInt()];
  buf = "";

  Serial.print("SSID: ");
  cmd_input(buf);
  net.ssid = buf.c_str();
  buf = "";

  Serial.print("Password: ");
  cmd_input(buf);
  net.password = buf.c_str();
  buf = "";
  net.security = WPA2_ACCESS_POINT;
}