#include "mqtt.h"

// IPAddress ip(172, 16, 0, 100);
// IPAddress server(172, 16, 0, 2);

// void event_callback(char *topic, uint8_t *payload, size_t length)
// {
//   log_i("Message arrived (%s): ", topic);
//   for (size_t i = 0; i < length; i++)
//   {
//     while (i < length && i % 12)
//       Serial.print(payload[i], 16);
//     Serial.println();
//   }
// }

// void reconnect(MQTTClient *client)
// {
//   while (!client->connected())
//   {
//     log_i("Attempting MQTT connection...");
//     // Attempt to connect
//     if (client->connect("arduinoClient"))
//     {
//       log_i("connected");
//       client->publish("outTopic", "hello world");
//       client->subscribe("inTopic");
//     }
//     else
//     {
//       log_i("failed, rc=%d try again in 5 seconds", client->state());
//       vTaskDelay(5000);
//     }
//   }
// }

void mqtt_client_loop_task(void *_client)
{
  auto client = (MQTTClient *)_client;
  log_i("Stating loop...");
  for (;; client->loop())
  {
    if (!client->connected())
      client->reconnect();
  }

  vTaskDelete(NULL);
}

void setup_mqtt()
{
  mqtt.setClient(client);
  mqtt.setServer("public.mqtthq.com", 1883);
  mqtt.setCallback(message_callback);
  mqtt.setReconnectCallback(reconnect_callback);
  mqtt.begin();
}