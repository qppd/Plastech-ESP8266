#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi credentials
#define WIFI_SSID "QPPD"
#define WIFI_PASSWORD "Programmer136"

// Firebase credentials
#define API_KEY "AIzaSyBx9r5gmNMgi8V7CkuY868UA9VaSPzdMMI"
#define DATABASE_URL "https://plastech-5436f-default-rtdb.firebaseio.com/"

#define USER_EMAIL "sajedhm@gmail.com"
#define USER_PASSWORD "Admin1+"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void initFIREBASE() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // 1. Assign Firebase credentials
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // 2. Assign Email and Password
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // 3. Optional: Set callback for token refresh info
  config.token_status_callback = tokenStatusCallback;

  // 4. Recommended settings
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(4096);

  // 5. Begin Firebase with config and authentication
  Firebase.begin(&config, &auth);

  while (!Firebase.ready()) {
    delay(100);  
  }

  if (auth.token.uid.length() > 0) {
    Serial.print("Signed in as UID: ");
    Serial.println(auth.token.uid.c_str());
  } else {
    Serial.println("Sign-in failed.");
  }
}


void sendFIREBASEData(int bottle_large, int bottle_small, int bin_level, int total_rewards, int total_weight,
                      int coin_stock) {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Create JSON object
    FirebaseJson json;
    json.set("bottle_large", bottle_large);
    json.set("bottle_small", bottle_small);
    json.set("bin_level", bin_level);
    json.set("total_rewards", total_rewards);
    json.set("total_weight", total_weight);
    json.set("coin_stock", coin_stock);

    Serial.println("Sending bin data as JSON...");

    // Send JSON to Firebase
    if (Firebase.RTDB.setJSON(&fbdo, "/plastech/bin", &json)) {
      Serial.println("Bin data sent successfully!");
    } else {
      Serial.print("Error sending bin data: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
