#include "smart_stuff.h"

bool timeSynced = false;

/*
Initialize time settings and syncronize time
*/
void initTime() {
  // Set callback function when time get syncronized.
  sntp_set_time_sync_notification_cb(timeSyncCallback);

  // Set interval for SNTP sync.
  sntp_set_sync_interval(NTP_UPDATE_INTERVAL);

  configTzTime(TIMEZONE, NTP_SERVER_1, NTP_SERVER_2);
}


/*
Time sync callback
*/
void timeSyncCallback(struct timeval *tv) {
  DEBUG_PRINTLN("Time synchronized.");
  DEBUG_PRINTLN(ctime(&tv->tv_sec));
  timeSynced = true;
}


/*
Get uptime of the program
*/
String getUptime() {
  int64_t uptime = esp_timer_get_time();

  String s = "Uptime: ";

  // Microseconds to days
  int days = uptime / 86400000000;
  if(days) {
    s.concat(days);
    s.concat(" days,");
    uptime = uptime % 86400000000;
  }

  // Microseconds to hours
  short hours = uptime / 3600000000;
  if(hours < 10) {
    s.concat("0");
  }
  s.concat(hours);
  s.concat(":");
  uptime = uptime % 3600000000;

  // Microseconds to minutes
  short minutes = uptime / 60000000;
  if(minutes < 10) {
    s.concat("0");
  }
  s.concat(minutes);
  s.concat(":");
  uptime = uptime % 60000000;

  // Microseconds to seconds
  short seconds = uptime / 1000000;
  if(seconds < 10) {
    s.concat("0");
  }
  s.concat(seconds);

  return s;
}