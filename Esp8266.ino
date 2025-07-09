#include "NTP_CONFIG.h"
#include "FIREBASE_CONFIG.h"


void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP8266");

  initFIREBASE();
}

int bottle_large;
int bottle_small;
int bin_level;
int total_rewards;
int total_weight;
int coin_stock;

unsigned long lastTime = 0;

void loop() {

  if (Firebase.ready() && (millis() - lastTime > 60 * 1000 || lastTime == 0)) {
    lastTime = millis();

    sendMessage();
  }

  // if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
  //   sendDataPrevMillis = millis();

  //   bottle_large = bottle_large + 1;
  //   bottle_small = bottle_small + 1;
  //   bin_level = bin_level + 1;
  //   total_rewards = total_rewards + 1;
  //   total_weight = total_weight + 1;
  //   coin_stock = coin_stock + 1;

  //   sendFIREBASEData(bottle_large, bottle_small, bin_level, total_rewards, total_weight, coin_stock);
  // }
}