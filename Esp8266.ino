#include "FIREBASE_CONFIG.h"


void setup() {
  Serial.begin(9600);
  Serial.println("Starting ESP8266");


  initFIREBASE();
}

void loop() {

  int bottle_large;
  int bottle_small;
  int bin_level;
  int total_rewards;
  int total_weight;
  int coin_stock;

  sendFIREBASEData(bottle_large, bottle_small, bin_level, total_rewards, total_weight, coin_stock);
  
}