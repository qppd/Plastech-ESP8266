#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Firebase Add-ons
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi credentials
#define WIFI_SSID "QPPD"
#define WIFI_PASSWORD "Programmer136"

// Firebase project credentials
#define API_KEY "AIzaSyBx9r5gmNMgi8V7CkuY868UA9VaSPzdMMI"
#define DATABASE_URL "https://plastech-5436f-default-rtdb.firebaseio.com/"

#define USER_EMAIL "sajedhm@gmail.com"
#define USER_PASSWORD "Jedtala01+"

// Device Token for FCM
#define DEVICE_REGISTRATION_ID_TOKEN "ecmu-phbT7m-xUJg0Kp5pS:APA91bGIsGng07vZk6CjYQci8yX-_9lBZ3PnDo-Uggm3hV1JhFZYyphhUnBMT-weMixZGF1UWc_6KW80b0vZVYy1BVaS8dYyL4YZB-KPGgU2ATNSRtTK8zc"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void initFIREBASE() {
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Assign Firebase credentials
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Assign email and password
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Optional: Token status callback
  config.token_status_callback = tokenStatusCallback;

  // Recommended settings
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(4096);

  // Begin Firebase
  Firebase.begin(&config, &auth);

  // Wait until Firebase is ready
  while (!Firebase.ready()) {
    delay(100);
  }

  // Confirm login success
  if (auth.token.uid.length() > 0) {
    Serial.print("Signed in as UID: ");
    Serial.println(auth.token.uid.c_str());
  } else {
    Serial.println("Sign-in failed.");
  }
}

void sendFIREBASEData(int bottle_large, int bottle_small, int bin_level, int total_rewards, int total_weight,
                      int coin_stock) {
  // Create JSON object
  FirebaseJson json;
  json.set("bottle_large", bottle_large);
  json.set("bottle_small", bottle_small);
  json.set("bin_level", bin_level);
  json.set("total_rewards", total_rewards);
  json.set("total_weight", total_weight);
  json.set("coin_stock", coin_stock);

  Serial.println("Sending bin data as JSON...");

  // Send to Firebase
  if (Firebase.RTDB.setJSON(&fbdo, "/plastech/bin", &json)) {
    Serial.println("Bin data sent successfully!");
  } else {
    Serial.print("Error sending bin data: ");
    Serial.println(fbdo.errorReason());
  }
}

void sendMessage() {
  Serial.print("Send Firebase Cloud Messaging... ");

  FCM_HTTPv1_JSON_Message msg;
  msg.token = DEVICE_REGISTRATION_ID_TOKEN;

  msg.notification.body = "Notification body";
  msg.notification.title = "Notification title";

  FirebaseJson payload;
  payload.add("temp", "28");
  payload.add("unit", "celsius");
  payload.add("timestamp", "1609815454");

  msg.data = payload.raw();

  if (Firebase.FCM.send(&fbdo, &msg)) {
    Serial.printf("ok\n%s\n\n", Firebase.FCM.payload(&fbdo).c_str());
  } else {
    Serial.println(fbdo.errorReason());
  }
}
