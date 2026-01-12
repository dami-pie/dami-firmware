#include "mqtt.h"
#include "config/config_data.h"

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

const char cert_pem[] = CERT_PEM;

const char client_crt[] = CLIENT_CRT;

const char client_key[] = CLIENT_KEY;

void mqtt_client_loop_task(void *_client)
{
  auto client = (MQTTClient *)_client;
  log_i("Stating mqtt loop...");
  for (;; client->loop())
  {
    if (WiFi.status() == WL_CONNECTED && !client->connected())
      client->reconnect();
  }

  vTaskDelete(NULL);
}

IPAddress address;
void setup_mqtt(MQTTClient *mqtt)
{
  log_i("Setup mqtt server");
  client.setCACert(cert_pem);
  client.setCertificate(client_crt);
  client.setPrivateKey(client_key);
  // log_v("Added CA certificate");
  mqtt->setClient(client);
  if (address.fromString(config.broker.server.getBuffer()))
  {
    mqtt->setServer(address, config.broker.port);
    log_d("broker host defined as %s:%u", address.toString(), config.broker.port);
  }
  else
  {
    mqtt->setServer(config.broker.server.getBuffer(), config.broker.port);
    log_d("broker host defined as %s:%u", config.broker.server.getBuffer(), config.broker.port);
  }

  // log_d("Setup broker url mqtt://%s:%u", mqtt, config.broker.port);
  mqtt->setCallback(mqtt_message_callback);
  mqtt->setReconnectCallback(mqtt_reconnect_callback);
  log_i("Starting server...");
  log_i("%s", mqtt->begin() ? "Done!" : "Fail");
}
