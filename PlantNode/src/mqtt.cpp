#include "smart_stuff.h"

PubSubClient MQTTClient(espClient);

/*
Initialize MQTT settings and connection.
*/
void initMQTT() {
  MQTTClient.setServer(MQTT_SERVER, MQTT_PORT);
  MQTTClient.setCallback(readMQTTCallback);
  // Increase keep alive because it would expire during watering.
  MQTTClient.setKeepAlive(60);
  reconnect();
}


/*
Reconnect MQTT connection if disconnected
*/
void reconnect() {
  DEBUG_PRINT("Attempting MQTT connection...");
  // Attempt to connect
  if(MQTTClient.connect("ESP32Client", MQTT_USER, MQTT_PASS)) {
    DEBUG_PRINTLN("connected");
    // resubscribe
    for(int i = 0; i < (sizeof(MQTT_topics) / sizeof(MQTT_topics[0])); i++) {
      MQTTClient.subscribe(MQTT_topics[i]);
      DEBUG_PRINT("Subscribed to topic: ");
      DEBUG_PRINTLN(MQTT_topics[i]);
    }
  } else {
    int reason = MQTTClient.state();
    DEBUG_PRINT("failed, reason: ");
    if(reason == -2) DEBUG_PRINTLN("MQTT Connection failed");
    else if(reason == -4) DEBUG_PRINTLN("MQTT Connection timeout");
    else {
      DEBUG_PRINT("Unknown error: ");
      DEBUG_PRINTLN(MQTTClient.state());
    }
  }
}


/*
Read MQTT messages
*/
void readMQTTCallback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';

  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");
  DEBUG_PRINTLN((char*)payload);

  // Do action based on topic.
  if(strcmp(topic, MQTT_topics[0]) == 0) {
    setWateringThreshold(payload);
  }
  else if(strcmp(topic, MQTT_topics[1]) == 0) {
    setWateringDuration(payload);
  }
  else if(strcmp(topic, MQTT_topics[2]) == 0) {
    setWateringRepeat(payload);
  }
  else if(strcmp(topic, MQTT_topics[3]) == 0) {
    setWateringSoakTime(payload);
  }
  else if(strcmp(topic, MQTT_topics[4]) == 0) {
    setWateringInterval(payload);
  }
}
