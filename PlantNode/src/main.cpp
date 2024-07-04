#include "smart_stuff.h"

unsigned long lastSensorRead = 0;
unsigned long lastMQTTRetry = 0;

float humidity = 0;
float temperature = 0;
float luminosity = 0;
int soilMoisture = 0;

RunningAverage runningSoilMoisture = RunningAverage(RUNNING_AVERAGE_SIZE);

#ifdef STATS
  unsigned int sensorTimer = 0;
#endif

void setup() {
  #if defined(DEBUG) || defined(STATS)
    Serial.begin(9600);
  #endif

  // WiFi
  initWifi();

  // Time
  initTime();

  // Database
  initDatabase();

  // MQTT
  initMQTT();

  // Sensors
  initSensors();

  // Relay for water pump
  initWateringRelay();
}


void loop() {
  if((millis() - lastSensorRead) > SENSOR_READ_INTERVAL) {
    #ifdef STATS
      sensorTimer = millis();
    #endif

    if(!getLuminosity(&luminosity)) {
      storeValue("luminosity", luminosity);
    }

    if(!getHumidityAndTemperature(&humidity, &temperature)) {
      storeValue("humidity", humidity);
      storeValue("temperature", temperature);
    }

    if(!getSoilMoisture(&soilMoisture)) {
      storeValue("soilMoisture", soilMoisture);
      runningSoilMoisture.add(soilMoisture);
    }

    // Try if watering is needed.
    if(tryStartWatering(&runningSoilMoisture)) {
      storeValue("waterPump", 1);
      doWatering();
      storeValue("waterPump", 0);
    }

    lastSensorRead = millis();

    #ifdef STATS
      debugPrint();
    #endif
  }

  if(!MQTTClient.connected()) {
    if((millis() - lastMQTTRetry) > MQTT_RETRY_INTERVAL) {
      reconnect();

      lastMQTTRetry = millis();
    }
  }
  else {
    MQTTClient.loop();
  }
}


#ifdef STATS
  int headerCounter = 0;

  void debugPrint() {

    if ((headerCounter % 20) == 0) {
      headerCounter = 0;
      STATS_PRINTLN("Luminosity \tSoil \\ Avg \tTemperature \tHumidity \tRead time");
    }
    headerCounter++;

    STATS_PRINT(luminosity);
    STATS_PRINT(" lx");
    STATS_PRINT("\t");

    STATS_PRINT(soilMoisture);
    STATS_PRINT("  \\ ");
    STATS_PRINT(runningSoilMoisture.getAverage());
    STATS_PRINT("\t");

    STATS_PRINT(temperature);
    STATS_PRINT(" °C");
    STATS_PRINT("\t");

    STATS_PRINTDEC(humidity, 1);
    STATS_PRINT(" %RH");
    STATS_PRINT("\t");

    STATS_PRINT(lastSensorRead-sensorTimer);
    STATS_PRINT(" ms");
    STATS_PRINT("\t");
    STATS_PRINT("\t");

    STATS_PRINTLN(getUptime());
  }
#endif