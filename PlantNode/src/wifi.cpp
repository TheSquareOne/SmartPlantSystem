#include "smart_stuff.h"

WiFiClient espClient;

/*
Initiliaze WiFi settings and connection.
*/
void initWifi() {
  WiFi.disconnect(true);

  // Set wifi event callbacks
  WiFi.onEvent(WiFiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  // Connect wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  delay(5000);  // Wait little for the wifi connection
}


/*
WiFi connected callback
*/
void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  DEBUG_PRINTLN("WiFi connected.");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}


/*
WiFi disconnected callback
*/
void WiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  DEBUG_PRINTLN("WiFi disconnected!");
  DEBUG_PRINT("Reason: ");
  DEBUG_PRINTLN(info.wifi_sta_disconnected.reason);
  DEBUG_PRINTLN("Attempting to reconnect.");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

