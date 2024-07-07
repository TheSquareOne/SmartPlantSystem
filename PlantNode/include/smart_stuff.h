#ifndef SMART_H
#define SMART_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <esp_sntp.h>

#include <InfluxDbClient.h>
#include <PubSubClient.h>

#include <BH1750.h>
#include <DHT20.h>

#include "RunningAverage.h"
#include "secret.h"



// #define DEBUG
// #define VERBOSE
// #define STATS

#ifdef DEBUG
  #define DEBUG_PRINT(x)   Serial.print(x)
  #define DEBUG_PRINTDEC(x, y) Serial.print(x, y)
  #define DEBUG_PRINTLN(x) Serial.println(x)

  #ifdef VERBOSE
    #define VERBOSE_PRINT(x)   Serial.print(x)
    #define VERBOSE_PRINTDEC(x, y) Serial.print(x, y)
    #define VERBOSE_PRINTLN(x) Serial.println(x)
  #else
    #define VERBOSE_PRINT(x)  
    #define VERBOSE_PRINTDEC(x) 
    #define VERBOSE_PRINTLN(x) 
  #endif

#else
  #define DEBUG_PRINT(x)  
  #define DEBUG_PRINTDEC(x) 
  #define DEBUG_PRINTLN(x) 
  #define VERBOSE_PRINT(x)  
  #define VERBOSE_PRINTDEC(x) 
  #define VERBOSE_PRINTLN(x) 
#endif

#ifdef STATS
  #define STATS_PRINT(x)   Serial.print(x)
  #define STATS_PRINTDEC(x, y) Serial.print(x, y)
  #define STATS_PRINTLN(x) Serial.println(x)
#else
  #define STATS_PRINT(x)  
  #define STATS_PRINTDEC(x) 
  #define STATS_PRINTLN(x) 
#endif



#define RELAY_PIN 23                          // GPIO pin for relay
#define SDA_PIN 21                            // GPIO pin for I2C communication SDA pin
#define SCL_PIN 22                            // GPIO pin for I2C communication SCL pin
#define SOIL_MOISTURE_PIN 32                  // GPIO pin for soil moisture sensor

// General intervals
#define SENSOR_READ_INTERVAL 4000
#define MQTT_RETRY_INTERVAL 30000
#define WIFI_RECONNECT_INTERVAL 60000

// NTP settings
#define TIMEZONE "EET-2EEST,M3.5.0/3,M10.5.0/4"
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.nis.gov"
#define NTP_UPDATE_INTERVAL 86400000

// Timestamp precision of written data
#define WRITE_PRECISION WritePrecision::S
// Number of points that will be written to the database at once
#define BATCH_SIZE 30
// Maximum number of points in buffer. Buffer contains new data that will be written to the database and also data that failed to be written due to network failure or server overloading
#define BUFFER_SIZE 300
// Maximum time(in seconds) data will be held in buffer before points are written to the db
#define FLUSH_INTERVAL 120
// Default retry interval in sec, if not sent by server. Value 0 disables retrying
#define RETRY_INTERVAL 5
// Maximum retry interval in sec
#define MAX_RETRY_INTERVAL 300
// Maximum count of retry attempts of failed writes
#define MAX_RETRY_ATTEMPTS 3
// Timeout (ms) for reading server response. Default 5000
#define DB_HTTP_TIMEOUT 5000
// Whether HTTP connection should be kept open after initial communication. Usable for frequent writes/queries. Default false
#define DB_HTTP_CONNECTION_REUSE 0

// Size of the running average buffer.
#define RUNNING_AVERAGE_SIZE 50

// Default watering settings. These are used if settings can't be retrieved via MQTT.
#define DEFAULT_WATERING_THRESHOLD 2400
#define DEFAULT_WATERING_DURATION 2000
#define DEFAULT_WATERING_REPEAT 1
#define DEFAULT_WATERING_SOAK_TIME 0
#define DEFAULT_WATERING_INTERVAL 604800000

// Some security boundaries for watering.
#define MIN_WATERING_THRESHOLD 0
#define MAX_WATERING_THRESHOLD 3000
#define MIN_WATERING_DURATION (1*1000)              // 1s
#define MAX_WATERING_DURATION (5*1000)              // 5s
#define MIN_WATERING_REPEAT 1
#define MAX_WATERING_REPEAT 10
#define MIN_WATERING_INTERVAL (12*60*60*1000)       // 12h
#define MAX_WATERING_INTERVAL (30*24*60*60*1000)    // 30d

// Debug
#ifdef STATS
  void debugPrint();
#endif

// Setup functions
void initWifi();
void initTime();
void initMQTT();
void initDatabase();
void initSensors();

// Initialize sensors
void initLuminositySensor(BH1750::Mode mode, TwoWire *wire);
void initDHTSensor(TwoWire *wire);
void initSoilMoistureSensor ();

// Get sensor data
int getLuminosity(float *luminosity);
int getHumidityAndTemperature(float *humidity, float *temperature);
int getSoilMoisture(int *soilMoisture);

void initWateringRelay();
int tryStartWatering(RunningAverage* runningSoilMoisture);
void doWatering();

// WiFi functions
void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

// NTP and time functions
void timeSyncCallback(struct timeval *tv);
String getUptime();

// Database functions
void setDatabaseSettings();
void setDataPoints();

int storeValue(const char *tag, int value);
int storeValue(const char *tag, float value);

// MQTT functions
void readMQTTCallback(char* topic, byte* payload, unsigned int length);
void reconnect();

int setWateringThreshold(byte* payload);
int setWateringDuration(byte* payload);
int setWateringRepeat(byte* payload);
int setWateringSoakTime(byte* payload);
int setWateringInterval(byte* payload);

// Variables and Objects
extern bool timeSynced;
extern PubSubClient MQTTClient;
extern WiFiClient espClient;
extern InfluxDBClient databaseClient;
extern Point dataPoint_TemperatureSensor;
extern Point dataPoint_LuminositySensor;
extern Point dataPoint_HumiditySensor;
extern Point dataPoint_SoilMoistureSensor;
extern Point dataPoint_WaterPump;

#endif