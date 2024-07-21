#include "smart_stuff.h"

// Database client
InfluxDBClient databaseClient(INFLUXDB_URL, INFLUXDB_ORG, 
                              INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Datapoints
Point dataPoint_TemperatureSensor("temperature_sensor");
Point dataPoint_LuminositySensor("luminosity_sensor");
Point dataPoint_HumiditySensor("humidity_sensor");
Point dataPoint_SoilMoistureSensor("soil_moisture_sensor");
Point dataPoint_WaterPump("watering");


/*
Initialize database settings.
*/
void initDatabase() {
  setDatabaseSettings();
  setDataPoints();

  if(databaseClient.validateConnection()) {
    DEBUG_PRINT("Connected to InfluxDB: ");
    DEBUG_PRINTLN(databaseClient.getServerUrl());
  }
  else {
    DEBUG_PRINT("InfluxDB connection failed: ");
    DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
  }
}


/*
Set database settings.
*/
void setDatabaseSettings() {
  
  if(databaseClient.setHTTPOptions(HTTPOptions()
  .connectionReuse(DB_HTTP_CONNECTION_REUSE)
  .httpReadTimeout(DB_HTTP_TIMEOUT)
  )) {
    DEBUG_PRINTLN("Database HTTP options set.");
  }
  else {
    DEBUG_PRINT("Setting database HTTP options failed.\nReason:");
    DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
  }

  if(databaseClient.setWriteOptions(WriteOptions()
    .writePrecision(WRITE_PRECISION)
    .batchSize(BATCH_SIZE)
    .bufferSize(BUFFER_SIZE)
    .flushInterval(FLUSH_INTERVAL)
    .retryInterval(RETRY_INTERVAL)
    .maxRetryInterval(MAX_RETRY_INTERVAL)
    .maxRetryAttempts(MAX_RETRY_ATTEMPTS)
  )) {
    DEBUG_PRINTLN("Database write options set.");
  }
  else {
    DEBUG_PRINT("Setting database write options failed.\nReason:");
    DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
  }
}


/*
Add tag's to datapoints.
*/
void setDataPoints() {
  dataPoint_TemperatureSensor.addTag("device", "ESP32");
  dataPoint_TemperatureSensor.addTag("plant", "muorinkukka_hope");

  dataPoint_LuminositySensor.addTag("device", "ESP32");
  dataPoint_LuminositySensor.addTag("plant", "muorinkukka_hope");

  dataPoint_HumiditySensor.addTag("device", "ESP32");
  dataPoint_HumiditySensor.addTag("plant", "muorinkukka_hope");

  dataPoint_SoilMoistureSensor.addTag("device", "ESP32");
  dataPoint_SoilMoistureSensor.addTag("plant", "muorinkukka_hope");

  dataPoint_WaterPump.addTag("device", "ESP32");
  dataPoint_WaterPump.addTag("plant", "muorinkukka_hope");
}


/*
  Save data to database. Used for float values.
  Parameters:
  const char *tag = tag used to identify what data is being stored.
  float value = value that is being saved.

  Return:
  0 if data was saved succesfully
  1 if data was not saved.
*/
int storeValue(const char *tag, float value) {
  // Use timeSynced to see when time is finished syncing. Without sync timestamp will be something of year 1970.
  if(timeSynced) {
    if(!strcmp(tag, "luminosity")) {
      dataPoint_LuminositySensor.clearFields();
      dataPoint_LuminositySensor.setTime(time(nullptr));
      dataPoint_LuminositySensor.addField("value", value);
      if(!databaseClient.writePoint(dataPoint_LuminositySensor)) {
        DEBUG_PRINT("Database write failed. Reason: ");
        DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
        return 1;
      }
    }
    else if(!strcmp(tag, "humidity")) {
      dataPoint_HumiditySensor.clearFields();
      dataPoint_HumiditySensor.setTime(time(nullptr));
      dataPoint_HumiditySensor.addField("value", value);
      if(!databaseClient.writePoint(dataPoint_HumiditySensor)) {
        DEBUG_PRINT("Database write failed. Reason: ");
        DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
        return 1;
      }
    }
    else if(!strcmp(tag, "temperature")) {
      dataPoint_TemperatureSensor.clearFields();
      dataPoint_TemperatureSensor.setTime(time(nullptr));
      dataPoint_TemperatureSensor.addField("value", value);
      if(!databaseClient.writePoint(dataPoint_TemperatureSensor)) {
        DEBUG_PRINT("Database write failed. Reason: ");
        DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
        return 1;
      }
    }
    else {
      DEBUG_PRINTLN("Invalid tag for database write.");
      return 1;
    }

    return 0;
  }
  else {
    VERBOSE_PRINTLN("Time is not synced. Skipping database storing.");
    return 1;
  }
}


/*
  Save data to database. Used for integers values.
  Parameters:
  const char *tag = tag used to identify what data is being stored.
  int value = value that is being saved.

  Return:
  0 if data was saved succesfully
  1 if data was not saved.
*/
int storeValue(const char *tag, int value) {
  // Use timeSynced to see when time is finished syncing. Without sync timestamp will be something of year 1970.
  if(timeSynced) {
    if(!strcmp(tag, "soilMoisture")) {
      dataPoint_SoilMoistureSensor.clearFields();
      dataPoint_SoilMoistureSensor.setTime(time(nullptr));
      dataPoint_SoilMoistureSensor.addField("value", value);
      if(!databaseClient.writePoint(dataPoint_SoilMoistureSensor)) {
        DEBUG_PRINT("Database write failed. Reason: ");
        DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
        return 1;
      }
    }
    else if(!strcmp(tag, "waterPump")) {
      dataPoint_WaterPump.clearFields();
      dataPoint_WaterPump.setTime(time(nullptr));
      dataPoint_WaterPump.addField("value", value);
      if(!databaseClient.writePoint(dataPoint_WaterPump)) {
        DEBUG_PRINT("Database write failed. Reason: ");
        DEBUG_PRINTLN(databaseClient.getLastErrorMessage());
        return 1;
      }
    }
    else {
      DEBUG_PRINTLN("Invalid tag for database write.");
      return 1;
    }
    return 0;
  }
  else {
    VERBOSE_PRINTLN("Time is not synced. Skipping database storing.");
    return 1;
  }
}
