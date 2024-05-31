#include "main.h"

WiFiClient client;
MQTTClient mqtt;
SemaphoreHandle_t mutex;

void ui_task(void *args)
{
  for (uint32_t d;; vTaskDelay(20))
  {
    xSemaphoreTake(mutex, portMAX_DELAY);
    lv_timer_handler();
    xSemaphoreGive(mutex);
  }
}

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.setDebugOutput(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  for (size_t i = 0; i < 100; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
      break;
    else
      delay(100);
  }

  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();
  else
    log_i("Connected to %s!", WIFI_SSID);

  mqtt.setClient(client);
  mqtt.setServer("public.mqtthq.com", 1883);
  mqtt.setCallback([](char *topic, uint8_t *data, size_t size)
                   {
    
    char str[size];
    memcpy(str, (const char*)data, size);
    log_i("%s", str);
    lv_qrcode_update(ui_QRCodeLogin, data, size);
    ; });

  mqtt.setReconnectCallback([](MQTTClient *client)
                            { 
                              if(client) {
                                client->connect(WiFi.macAddress().c_str());
                                client->subscribe(SECURITY_KEY);
                              }
                              xSemaphoreTake(mutex, portMAX_DELAY);
                              yield();
                              show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);
                              xSemaphoreGive(mutex); });
  setup_screen();
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BROWN_COLOR);
  codeUpdate("wating...");

  mutex = xSemaphoreCreateMutex();
  mqtt.begin();
  xTaskCreate(ui_task, "ui", 8192U, NULL, 1, &ui_wacher);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED && mqtt.connected())
  {
    show_layout(LV_SYMBOL_WIFI "\tConectado", GREEN_COLOR);
    // codeUpdate(
    //     otp.getCode(
    //         mktime(&timeInfo)));
  }
  else
    show_layout(LV_SYMBOL_CLOSE "\tDesconectado", RED_COLOR);

  getUpdate();
  delay(1000);
  // vTaskDelay(500 / portTICK_PERIOD_MS);
}