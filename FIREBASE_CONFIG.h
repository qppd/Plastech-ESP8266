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

#define FIREBASE_PROJECT_ID "plastech-5436f"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-fbsvc@plastech-5436f.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDjbVBWMNUj2sE2\nhreNyaaLOMBaPf5nE4sC+UOvakVrRAwdCZfHhRQtE7586XY9wXiKpkN/eObDo4Fq\nw4XoHt1R5WaTHFaog+CNxQVFIkEg/1zoktx6XVwJDjfgSk0XYLy/djNeg25GdNfA\n0T0mZLXVh9nBYX/JdKRGOr++flw54nf3C0USPNB6iGSxE4NqMsP/B/tO6LRvPiED\neX8UjcSFGD/rRxXDCX3BBXO2dDlPXHFX0hOxhKJmtJh/OGBbrUytMe21FTjcOTBV\naYlR0dFEpfHdrsxdeBDqXAOGMHCBHB4oo03syEWY4+wN2SsSNy8f8xBI4YhaaVt0\nBHaIpFZhAgMBAAECggEAAxP8rV311Ilmag2CuHmNNn8rI02Jfu2lyQGEqAAhNmVy\nYIY7tqiXcFC/GEO0pPtUXVQIv9XsEaPJ4Z8J1e1tA2m+bKlKhvdZ97E99ASVqxOp\nt0Admc5fq4O0v+Vhi7p0c4bvBQ10VaHJNcPfNwDP8TbofJSMPTbF7mlnJPXi26mt\ne4DdfaPGH37jhZTA1xJM5MC671Agea9rPXkU115c/VRvXUs0eZt7Zuzwd5w3C3KW\nHfEBjXqfxlBawAqbQUpjkN6qIcRlpYy178YmiulwxwzsM5+A0+zTZcnJVgP8ZAsm\nCKnCzsUt8qWHaUByi9KMw6IcVLxTsG6hxS4SHHLM0QKBgQD2rp1pGrR4jqfJSOsd\nZdXPCy1N58gVklervNGenH6NdFefxg5kUNN/rNI7/kX9RZEgwOyNoWuc3IU+anek\nZseL2kPFzcHlJn3QRRshuPOWwoHSnZnQqCYtc/6S/MAnkd8NGD97h5CmKx8oNy9e\nmJe5FoLfBNlCpQpVdLIyfjsyTQKBgQDsBIEysZydZj76g38mSLlSvIyNpJWR7fV8\nz6d/XXKu7bUrVdUwM5geSTTWAnIogoqAorG4JD/pRgjypKsArSKxKgZmHpYqdB0T\nJxThFdeuGb/mhkzYFdES6AaH+MAdDoquaaFy/+ru2P7CdXEADxjMR4a28CUC0dnq\nDmAJA4J2ZQKBgQCPPFj+fJkqH3thr0uGKGl77TuDLwDJNc5eVCdLeAcrz9BJX9Ff\ngmcsjYcNetnc9JYOdLdmjcJUtN9RnLSX624W0MflhVH+4tjPJDFa5W/brFmBmC2G\nMPzqC55YbxaMY3Emw1upONBPDF7tFQ4WWXhP6rhXExOsVPSpJjeKhi2NSQKBgBEf\nxov6DQXyjDE65zfM4P1axsvkHCIJZN1YZ6u7CP4eKx3ozV4A8FGKODYq2UONLSVm\nKITWxHhVZ4LLZcjXxwOZq/suzvBnmlBwbnhiUFLGdYDxT5QCCThY81hcOPwiuPcs\nnNUuMkqAHgPX54n8jm0L539Qby8EMM5lmpws0ANtAoGAeg9P8tuhDE3Wnm7l1HG2\n28k+vz/GJtZE83zT8rH9X/LlDb9kPCpfEqdteM7SxhpPGrZb3f+SNpVKqhB9P7Av\nx+OceUw7zimZ3lt3kVxnINoCnMlHTmqyJG8Gs9CFw8ZjXgHAGSug2XIgpRkB7wLQ\nx1GSAgt32yFyWr/uWM62hkk=\n-----END PRIVATE KEY-----\n";

#define USER_EMAIL "sajedhm@gmail.com"
#define USER_PASSWORD "Jedtala01+"

// Device Token for FCM
#define DEVICE_REGISTRATION_ID_TOKEN "fho5o_l-SrOaUVkrrp4Af9:APA91bHxdYtuEyTATtPiv8bnXlYYtFZ_JKjKSpoqk1RPElBz6Ebe0NmkPLTZ2Embn8PYEqiB1DA_OrQ1WPgixyF0s-FbyXMKNfmMmLphXoMSqaS06xXePBo"

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
