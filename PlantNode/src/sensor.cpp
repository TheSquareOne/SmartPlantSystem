#include "smart_stuff.h"

BH1750 luminositySensor;
DHT20 DHTSensor;


/*
Initialize all the sensors, pins and I2C bus.
*/
void initSensors() {
    Wire.begin(SDA_PIN, SCL_PIN);   // Set I2C bus

    initLuminositySensor(BH1750::CONTINUOUS_HIGH_RES_MODE, &Wire);
    initDHTSensor(&Wire);
    initSoilMoistureSensor();

    DEBUG_PRINTLN("All sensors initialized!");
}


/*
Initialize luminosity sensor.

Uses I2C bus.
*/
void initLuminositySensor(BH1750::Mode mode, TwoWire *wire) {
    luminositySensor.begin(mode, 0x23, wire);       // Start BH1750 sensor

    DEBUG_PRINTLN("Luminosity sensor initialized.");
}


/*
Read luminosity value to variables provided in parameter.

Return 0 on success.
*/
int getLuminosity(float *luminosity) {
    *luminosity = luminositySensor.readLightLevel();
    VERBOSE_PRINT("Luminosity: ");
    VERBOSE_PRINT(*luminosity);
    VERBOSE_PRINTLN(" lx");
    return 0;
}


/*
Initialize DHT20 sensor.

Uses I2C bus.
*/
void initDHTSensor(TwoWire *wire) {
    DHTSensor = DHT20(wire);
    DHTSensor.begin();      // Start DHT20 sensor

    DHTSensor.requestData();    // Start first data request

    DEBUG_PRINTLN("DHT20 sensor initialized.");
}


/*
Pass two pointers as a parameters. Humidity and temperature values are stored in to these parameters.

Return 0 if succesful.
Return 1 if there was error.
*/
int getHumidityAndTemperature(float *humidity, float *temperature) {
    // Read and convert data. Check for possible error.
    if(int status = DHTSensor.read()) {
        switch(status) {
            case DHT20_ERROR_CHECKSUM:
                DEBUG_PRINTLN("DHT20_ERROR_CHECKSUM");
                break;
            case DHT20_ERROR_CONNECT:
                DEBUG_PRINTLN("DHT20_ERROR_CONNECT");
                break;
            case DHT20_MISSING_BYTES:
                DEBUG_PRINTLN("DHT20_MISSING_BYTES");
                break;
            case DHT20_ERROR_BYTES_ALL_ZERO:
                DEBUG_PRINTLN("DHT20_ERROR_BYTES_ALL_ZERO");
                break;
            case DHT20_ERROR_READ_TIMEOUT:
                DEBUG_PRINTLN("DHT20_ERROR_READ_TIMEOUT");
                break;
            case DHT20_ERROR_LASTREAD:
                DEBUG_PRINTLN("DHT20_ERROR_LASTREAD");
                break;
        }
        return 1;
    }

    *humidity = DHTSensor.getHumidity();
    VERBOSE_PRINT("Humidity: ");
    VERBOSE_PRINT(*humidity);
    VERBOSE_PRINTLN(" %RH");

    *temperature = DHTSensor.getTemperature();
    VERBOSE_PRINT("Temperature: ");
    VERBOSE_PRINT(*temperature);
    VERBOSE_PRINTLN(" °C");

    DHTSensor.requestData();    // Request new data

    return 0;
}


/*
Initialize soil moisture sensor.
*/
void initSoilMoistureSensor () {
    pinMode(SOIL_MOISTURE_PIN, INPUT);

    DEBUG_PRINTLN("Soil moisture sensor pin set.");
}


/*
Read AD-converted soil moisture value to variable provided as a parameter.

Return 0 on success.
*/
int getSoilMoisture(int *soilMoisture) {
    *soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    VERBOSE_PRINT("Soil moisture: ");
    VERBOSE_PRINTLN(*soilMoisture);
    return 0;
}