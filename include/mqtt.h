#if !defined(__mqtt_client_handle_h__)
#define __mqtt_client_handle_h__

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

void mqtt_client_loop_task(void *_client);

class MQTTClient : public PubSubClient
{
  // typedef void (*event_callback_t)(char *, uint8_t *, size_t);
  typedef void (*reconnect_callback_t)(MQTTClient *);
  struct
  {
    TaskHandle_t *handle;
    BaseType_t priority = 2;
    BaseType_t result;
  } task;

  reconnect_callback_t reconnect_callback;

public:
  void setReconnectCallback(reconnect_callback_t cb)
  {
    reconnect_callback = cb;
  }

  void reconnect()
  {
    if (reconnect_callback)
    {
      log_i("Triing to reconnect...");
      reconnect_callback(this);
    }
  }

  bool begin()
  {
    task.result = xTaskCreate(
        mqtt_client_loop_task,
        "mqtt_client_task",
        8 * 1024,
        this,
        task.priority,
        task.handle);

    return task.result == pdPASS;
  }
};

void setup_mqtt();

void message_callback(char *topic, uint8_t *data, size_t size);
void reconnect_callback(MQTTClient *);

extern WiFiClient client;
extern MQTTClient mqtt;

#endif // __mqtt_client_handle_h__
