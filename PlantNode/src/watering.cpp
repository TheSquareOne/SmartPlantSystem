#include "smart_stuff.h"

#ifdef VERBOSE
  unsigned int wateringTime = 0;
#endif

// Settings defining for watering is done.
unsigned int wateringThreshold = DEFAULT_WATERING_THRESHOLD;
unsigned int wateringDuration = DEFAULT_WATERING_DURATION;
unsigned int wateringRepeat = DEFAULT_WATERING_REPEAT;
unsigned int wateringSoakTime = DEFAULT_WATERING_SOAK_TIME;
unsigned int wateringInterval = DEFAULT_WATERING_INTERVAL;

unsigned int lastWatering = 0;


/*
Initialize relay that handles water pump.
*/
void initWateringRelay() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}


/*
Check if there is any actions needed regarding watering.

Return 0 if no actions needed.
Return 1 if watering started.
*/
int tryStartWatering(RunningAverage* runningSoilMoisture) {
  if((*runningSoilMoisture).isFull() && 
          (lastWatering == 0 || wateringInterval <= (millis() - lastWatering)) &&
          wateringThreshold <= (*runningSoilMoisture).getAverage()) {

    return 1;
  }
  return 0;
}


/*
This function does the watering.
*/
void doWatering() {

  #ifdef VERBOSE
    wateringTime = millis();
  #endif
  VERBOSE_PRINTLN("Started watering.");

  int repeat = 0;
  do {
    // If there is no repeats and we water once, soaking time is not needed.
    if(repeat) {
      delay(wateringSoakTime);
    }
    digitalWrite(RELAY_PIN, LOW);   // Set relay to "LOW", which turns the water pump ON.
    delay(wateringDuration);
    digitalWrite(RELAY_PIN, HIGH);  // Set relay to "HIGH", which turns the water pump OFF.
    repeat++;
  }
  while(repeat < wateringRepeat);

  VERBOSE_PRINT("Stopped watering. Duration: ");
  VERBOSE_PRINT((millis()-wateringTime));
  VERBOSE_PRINTLN("ms");
  lastWatering = millis();
}


/*
Set new watering threshold from MQTT message.

Return 0 if threshold was changed.
Return 1 if value was invalid and threshold not changed.
*/
int setWateringThreshold(byte* payload) {
  char* endPointer;
  int value = strtol((char*)payload, &endPointer, 10);  // strtol converts payload to integer.

  // If *endPointer isn't null, payload isn't only numbers and considered invalid
  if(*endPointer != '\0') {
    DEBUG_PRINTLN("Not integer.");
    return 1;
  }

  if(value <= MAX_WATERING_THRESHOLD && value >= MIN_WATERING_THRESHOLD) {

    wateringThreshold = value;
    DEBUG_PRINT("New watering threshold set: ");
    DEBUG_PRINTLN(value);
    return 0;
  }
  else {
    DEBUG_PRINTLN("Invalid watering threshold value.");
    return 1;
  }
}


/*
Set new watering duration from MQTT message.

Return 0 if duration was changed.
Return 1 if value was invalid and duration not changed.
*/
int setWateringDuration(byte* payload) {
  char* endPointer;
  int value = strtol((char*)payload, &endPointer, 10);  // strtol converts payload to integer.

  // If *endPointer isn't null, payload isn't only numbers and considered invalid
  if(*endPointer != '\0') {
    DEBUG_PRINTLN("Not integer.");
    return 1;
  }

  value = value * 1000;   // Seconds to milliseconds.

  // Set some boundaries.
  if(value <= MAX_WATERING_DURATION && value >= MIN_WATERING_DURATION) {
    wateringDuration = value;
    DEBUG_PRINT("New watering duration set: ");
    DEBUG_PRINTLN(value);
    return 0;
  }
  else {
    DEBUG_PRINTLN("Invalid watering duration value.");
    return 1;
  }
}


/*
Set a new watering interval so that watering doesn't occure too often. Payload must contain number and character.
Accepted characters:
h = hour
d = day
w = week

example: 24h, 5d, 2w

Return 0 if interval was changed.
Return 1 if value was invalid and interval not changed.
*/
int setWateringInterval(byte* payload) {
  int len = strlen((char*)payload)-1;   // -1 because of '\0'.
  int value = 0;
  char format = payload[len];

  if((isDigit(payload[0]) && isAlpha(format)) && (format == 'h' || format == 'd' || format == 'w')) {
    for(int i = 0; i < len; i++) {
      if(isDigit(payload[i])) {
        value = value * 10 + (payload[i] - '0');
      }
      else {
        DEBUG_PRINTLN("Invalid input.");
        return 1;
      }
    }

    DEBUG_PRINT("Value is: ");
    DEBUG_PRINTLN(value);
    DEBUG_PRINT("Format is: ");
    DEBUG_PRINTLN(format);

    // Convert the desired interval to ms.
    if(format == 'h') wateringInterval = value * 60 * 60 * 1000;
    else if(format == 'd') wateringInterval = value * 24 * 60 * 60 * 1000;
    else if(format == 'w') wateringInterval = value * 7 * 24 * 60 * 60 * 1000;
    else {
      DEBUG_PRINTLN("Error, format must be h, d or w.");
      return 1;
    }
  }
  else {
    DEBUG_PRINTLN("Invalid input. Input doesn't start with digit or end with correct character.");
    return 1;
  }

  DEBUG_PRINT("New watering interval set: ");
  DEBUG_PRINTLN(wateringInterval);
  return 0;
}


/*
Set new watering repeat amount from MQTT message.

Return 0 if repeat was changed.
Return 1 if value was invalid and not changed.
*/
int setWateringRepeat(byte* payload) {
  char* endPointer;
  int value = strtol((char*)payload, &endPointer, 10);  // strtol converts payload to integer.

  // If *endPointer isn't null, payload isn't only numbers and considered invalid
  if(*endPointer != '\0') {
    DEBUG_PRINTLN("Not integer.");
    return 1;
  }

  if(value <= MAX_WATERING_REPEAT && value >= MIN_WATERING_REPEAT) {

    wateringRepeat = value;
    DEBUG_PRINT("New watering repeat set: ");
    DEBUG_PRINTLN(value);
    return 0;
  }
  else {
    DEBUG_PRINTLN("Invalid watering repeat value.");
    return 1;
  }
}


/*
Set new watering soak time from MQTT message.
Soak time is time to wait between repeats on watering.
This is used to prevent water from overflowing from the pot and let water soak in to soil between repeated watering.

Return 0 if soak time was changed.
Return 1 if value was invalid and soak time not changed.
*/
int setWateringSoakTime(byte* payload) {
  int len = strlen((char*)payload)-1;   // -1 because of '\0'.
  int value = 0;
  char format = payload[len];

  if((isDigit(payload[0]) && isAlpha(format)) && (format == 's' || format == 'm' || format == 'h')) {
    for(int i = 0; i < len; i++) {
      if(isDigit(payload[i])) {
        value = value * 10 + (payload[i] - '0');
      }
      else {
        DEBUG_PRINTLN("Invalid input.");
        return 1;
      }
    }

    DEBUG_PRINT("Value is: ");
    DEBUG_PRINTLN(value);
    DEBUG_PRINT("Format is: ");
    DEBUG_PRINTLN(format);

    // Convert the desired interval to ms.
    if(format == 's') wateringSoakTime = value * 1000;
    else if(format == 'm') wateringSoakTime = value * 60 * 1000;
    else if(format == 'h') wateringSoakTime = value * 60 * 60 * 1000;
    else {
      DEBUG_PRINTLN("Error, format must be s, m or h.");
      return 1;
    }
  }
  else {
    DEBUG_PRINTLN("Invalid input. Input doesn't start with digit or end with correct character.");
    return 1;
  }

  DEBUG_PRINT("New watering soak time set: ");
  DEBUG_PRINTLN(wateringSoakTime);
  return 0;
}
