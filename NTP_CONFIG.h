#include <time.h>

String DATETIME = "";

// Week Days
String weekDays[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// Month Names
String months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

void initNTP() {
  // Set timezone to GMT+8 (28800 seconds)
  configTime(28800, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync");
  struct tm timeinfo;
  int retries = 0;
  while (!getLocalTime(&timeinfo) && retries < 10) {
    Serial.print(".");
    delay(1000);
    retries++;
  }

  if (retries >= 10) {
    Serial.println("\nFailed to get time from NTP");
  } else {
    Serial.println("\nTime synced from NTP");
  }
}

void getNTPDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", &timeinfo);
  DATETIME = String(timeStringBuff);
  Serial.println(DATETIME);
}

