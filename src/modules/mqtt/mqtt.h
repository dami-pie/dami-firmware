#if !defined(__mqtt_client_handle_h__)
#define __mqtt_client_handle_h__

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"

void mqtt_client_loop_task(void *_client);

typedef enum
{
  MQ_DISCONNECTED = MQTT_DISCONNECTED,
  MQ_CONNECTED = MQTT_CONNECTED,
  MQ_CONNECTION_TIMEOUT = MQTT_CONNECTION_TIMEOUT,
  MQ_CONNECTION_LOST = MQTT_CONNECTION_LOST,
  MQ_CONNECT_FAILED = MQTT_CONNECT_FAILED,
  MQ_CONNECT_BAD_PROTOCOL = MQTT_CONNECT_BAD_PROTOCOL,
  MQ_CONNECT_BAD_CLIENT_ID = MQTT_CONNECT_BAD_CLIENT_ID,
  MQ_CONNECT_UNAVAILABLE = MQTT_CONNECT_UNAVAILABLE,
  MQ_CONNECT_BAD_CREDENTIALS = MQTT_CONNECT_BAD_CREDENTIALS,
  MQ_CONNECT_UNAUTHORIZED = MQTT_CONNECT_UNAUTHORIZED,
  MQ_RECONNECTING = 10,
} mqtt_client_status_t;
class MQTTClient : public PubSubClient
{
  // typedef void (*event_callback_t)(char *, uint8_t *, size_t);
  typedef void (*reconnect_callback_t)(MQTTClient *);
  struct
  {
    TaskHandle_t handle;
    BaseType_t priority = 2;
    BaseType_t result;
  } task;
  bool reconnecting;
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
      reconnecting = true;
      log_i("Trying to reconnect...");
      reconnect_callback(this);
      reconnecting = false;
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
        &task.handle);

    return task.result == pdPASS;
  }

  void pause() { vTaskSuspend(task.handle); }
  void resume() { vTaskResume(task.handle); }

  mqtt_client_status_t status()
  {
    if (reconnecting)
      return MQ_RECONNECTING;
    else
      switch (this->state())
      {
      case MQTT_DISCONNECTED:
        return MQ_DISCONNECTED;
      case MQTT_CONNECTED:
        return MQ_CONNECTED;
      case MQTT_CONNECTION_TIMEOUT:
        return MQ_CONNECTION_TIMEOUT;
      case MQTT_CONNECTION_LOST:
        return MQ_CONNECTION_LOST;
      case MQTT_CONNECT_FAILED:
        return MQ_CONNECT_FAILED;
      case MQTT_CONNECT_BAD_PROTOCOL:
        return MQ_CONNECT_BAD_PROTOCOL;
      case MQTT_CONNECT_BAD_CLIENT_ID:
        return MQ_CONNECT_BAD_CLIENT_ID;
      case MQTT_CONNECT_UNAVAILABLE:
        return MQ_CONNECT_UNAVAILABLE;
      case MQTT_CONNECT_BAD_CREDENTIALS:
        return MQ_CONNECT_BAD_CREDENTIALS;
      case MQTT_CONNECT_UNAUTHORIZED:
        return MQ_CONNECT_UNAUTHORIZED;
      default:
        return MQ_CONNECT_FAILED;
      }
  }
};

void setup_mqtt(MQTTClient *);

void mqtt_message_callback(char *topic, uint8_t *data, size_t size);
void mqtt_reconnect_callback(MQTTClient *);

extern WiFiClientSecure client;
// extern WiFiClient client;
extern MQTTClient mqtt;

#endif // __mqtt_client_handle_h__
