#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Firebase Add-ons
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define MAX_TOKENS 10  // adjust as needed

String deviceTokens[MAX_TOKENS];
int tokenCount = 0;


// Wi-Fi credentials


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

FirebaseData tokenStream;

String tokenParentPath = "plastech";
String tokenPaths[1] = { "/tokens" };


unsigned long sendDataPrevMillis = 0;


void tokenStreamCallback(MultiPathStream stream) {
  // Check if the stream updated /tokens
  if (stream.get("/tokens")) {
    Serial.println("token Updated Path: " + stream.dataPath);
    Serial.println("token New Value: " + stream.value);

    FirebaseJson json;
    FirebaseJsonData result;

    // Clear any previous JSON content before loading new data
    json.clear();
    json.setJsonData(stream.value);

    // Begin iterating through all children under /tokens
    size_t count = json.iteratorBegin();
    tokenCount = 0;  // reset token count before filling

    for (size_t i = 0; i < count; i++) {
      FirebaseJson::IteratorValue value = json.valueAt(i);
      String pushID = value.key;

      // Skip any key that doesn't contain a nested object
      if (!value.value.startsWith("{")) {
        Serial.println("Ignored non-object key: " + pushID);
        continue;
      }

      String fullPath = pushID + "/device_token";

      if (json.get(result, fullPath)) {
        String deviceToken = result.stringValue;
        Serial.println("✅ Extracted device_token from " + pushID + ": " + deviceToken);

        // Add to array if not full
        if (tokenCount < MAX_TOKENS) {
          deviceTokens[tokenCount++] = deviceToken;
        } else {
          Serial.println("⚠️  Token list full, cannot store more.");
        }

      } else {
        Serial.println("⚠️  Skipping key (no device_token): " + pushID);
      }
    }



    // End iteration
    json.iteratorEnd();
  }
}



void tokenStreamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("token stream timed out, attempting to resume...");  // Notify of timeout
  }
  if (!tokenStream.httpConnected()) {
    Serial.printf("token Error code: %d, reason: %s\n", tokenStream.httpCode(), tokenStream.errorReason().c_str());  // Output HTTP error details if disconnected
  }
}

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

  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = PRIVATE_KEY;

  // Assign email and password
  // auth.user.email = USER_EMAIL;
  // auth.user.password = USER_PASSWORD;

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

  Serial.println("Starting token stream");
  if (!Firebase.RTDB.beginMultiPathStream(&tokenStream, tokenParentPath)) {
    Serial.printf("token stream initialization failed: %s\n", tokenStream.errorReason().c_str());  // Output error if stream setup fails
  } else {
    Firebase.RTDB.setMultiPathStreamCallback(&tokenStream, tokenStreamCallback, tokenStreamTimeoutCallback);  // Assign callback functions for stream
    Serial.println("Firebase token stream initialized successfully!");                                        // Output success message for stream initialization
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

void sendMessage(String title, String body) {
  Serial.print("Send Firebase Cloud Messaging... ");

  FCM_HTTPv1_JSON_Message msg;
  msg.token = DEVICE_REGISTRATION_ID_TOKEN;

  msg.notification.body = title;
  msg.notification.title = body;

  FirebaseJson payload;
  payload.add("status", "1");

  msg.data = payload.raw();

  if (Firebase.FCM.send(&fbdo, &msg)) {
    Serial.printf("ok\n%s\n\n", Firebase.FCM.payload(&fbdo).c_str());
  } else {
    Serial.println(fbdo.errorReason());
  }
}

void sendMessageToAll(String title, String body) {
  for (int i = 0; i < tokenCount; i++) {
    Serial.println("📲 Sending to: " + deviceTokens[i]);

    FCM_HTTPv1_JSON_Message msg;
    msg.token = deviceTokens[i];
    msg.notification.title = title;
    msg.notification.body = body;

    FirebaseJson payload;
    payload.add("status", "1");
    msg.data = payload.raw();

    if (Firebase.FCM.send(&fbdo, &msg)) {
      Serial.println("Sent!");
    } else {
      Serial.println("Error: " + fbdo.errorReason());
    }
  }
}
