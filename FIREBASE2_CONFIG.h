#define ENABLE_USER_AUTH
#define ENABLE_SERVICE_AUTH
#define ENABLE_DATABASE
#define ENABLE_MESSAGING

#include <FirebaseClient.h>
#include "ExampleFunctions.h"

// Wi-Fi
#define WIFI_SSID "QPPD"
#define WIFI_PASSWORD "Programmer136"

// Firebase Auth
#define API_KEY "AIzaSyBx9r5gmNMgi8V7CkuY868UA9VaSPzdMMI"
#define DATABASE_URL "https://plastech-5436f-default-rtdb.firebaseio.com/"
#define USER_EMAIL "sajedhm@gmail.com"
#define USER_PASSWORD "Jedtala01+"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-fbsvc@plastech-5436f.iam.gserviceaccount.com"
#define FIREBASE_PROJECT_ID "plastech-5436f"
#define DEVICE_REGISTRATION_ID_TOKEN "fho5o_l-SrOaUVkrrp4Af9:APA91bHxdYtuEyTATtPiv8bnXlYYtFZ_JKjKSpoqk1RPElBz6Ebe0NmkPLTZ2Embn8PYEqiB1DA_OrQ1WPgixyF0s-FbyXMKNfmMmLphXoMSqaS06xXePBo"

const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDjbVBWMNUj2sE2\nhreNyaaLOMBaPf5nE4sC+UOvakVrRAwdCZfHhRQtE7586XY9wXiKpkN/eObDo4Fq\nw4XoHt1R5WaTHFaog+CNxQVFIkEg/1zoktx6XVwJDjfgSk0XYLy/djNeg25GdNfA\n0T0mZLXVh9nBYX/JdKRGOr++flw54nf3C0USPNB6iGSxE4NqMsP/B/tO6LRvPiED\neX8UjcSFGD/rRxXDCX3BBXO2dDlPXHFX0hOxhKJmtJh/OGBbrUytMe21FTjcOTBV\naYlR0dFEpfHdrsxdeBDqXAOGMHCBHB4oo03syEWY4+wN2SsSNy8f8xBI4YhaaVt0\nBHaIpFZhAgMBAAECggEAAxP8rV311Ilmag2CuHmNNn8rI02Jfu2lyQGEqAAhNmVy\nYIY7tqiXcFC/GEO0pPtUXVQIv9XsEaPJ4Z8J1e1tA2m+bKlKhvdZ97E99ASVqxOp\nt0Admc5fq4O0v+Vhi7p0c4bvBQ10VaHJNcPfNwDP8TbofJSMPTbF7mlnJPXi26mt\ne4DdfaPGH37jhZTA1xJM5MC671Agea9rPXkU115c/VRvXUs0eZt7Zuzwd5w3C3KW\nHfEBjXqfxlBawAqbQUpjkN6qIcRlpYy178YmiulwxwzsM5+A0+zTZcnJVgP8ZAsm\nCKnCzsUt8qWHaUByi9KMw6IcVLxTsG6hxS4SHHLM0QKBgQD2rp1pGrR4jqfJSOsd\nZdXPCy1N58gVklervNGenH6NdFefxg5kUNN/rNI7/kX9RZEgwOyNoWuc3IU+anek\nZseL2kPFzcHlJn3QRRshuPOWwoHSnZnQqCYtc/6S/MAnkd8NGD97h5CmKx8oNy9e\nmJe5FoLfBNlCpQpVdLIyfjsyTQKBgQDsBIEysZydZj76g38mSLlSvIyNpJWR7fV8\nz6d/XXKu7bUrVdUwM5geSTTWAnIogoqAorG4JD/pRgjypKsArSKxKgZmHpYqdB0T\nJxThFdeuGb/mhkzYFdES6AaH+MAdDoquaaFy/+ru2P7CdXEADxjMR4a28CUC0dnq\nDmAJA4J2ZQKBgQCPPFj+fJkqH3thr0uGKGl77TuDLwDJNc5eVCdLeAcrz9BJX9Ff\ngmcsjYcNetnc9JYOdLdmjcJUtN9RnLSX624W0MflhVH+4tjPJDFa5W/brFmBmC2G\nMPzqC55YbxaMY3Emw1upONBPDF7tFQ4WWXhP6rhXExOsVPSpJjeKhi2NSQKBgBEf\nxov6DQXyjDE65zfM4P1axsvkHCIJZN1YZ6u7CP4eKx3ozV4A8FGKODYq2UONLSVm\nKITWxHhVZ4LLZcjXxwOZq/suzvBnmlBwbnhiUFLGdYDxT5QCCThY81hcOPwiuPcs\nnNUuMkqAHgPX54n8jm0L539Qby8EMM5lmpws0ANtAoGAeg9P8tuhDE3Wnm7l1HG2\n28k+vz/GJtZE83zT8rH9X/LlDb9kPCpfEqdteM7SxhpPGrZb3f+SNpVKqhB9P7Av\nx+OceUw7zimZ3lt3kVxnINoCnMlHTmqyJG8Gs9CFw8ZjXgHAGSug2XIgpRkB7wLQ\nx1GSAgt32yFyWr/uWM62hkk=\n-----END PRIVATE KEY-----\n";

// Firebase Objects
SSL_CLIENT ssl_client;
AsyncClientClass aClient(ssl_client);
FirebaseApp app;
RealtimeDatabase Database;
Messaging messaging;
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD, 3000);
ServiceAuth sa_auth(FIREBASE_CLIENT_EMAIL, FIREBASE_PROJECT_ID, PRIVATE_KEY, 3000);
AsyncResult databaseResult, fcmResult;

bool firebaseReady = false;
bool taskComplete = false;

void initFIREBASE() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected with IP: " + WiFi.localIP().toString());

  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  set_ssl_client_insecure_and_buffer(ssl_client);

  app.setTime(get_ntp_time());
  Serial.println("Initializing Firebase App...");

  // Initialize App
  initializeApp(aClient, app, getAuth(sa_auth), auth_debug_print, "🔐 ServiceAuth");
  app.getApp(Database);
  app.getApp(messaging);

  Database.url(DATABASE_URL);
  firebaseReady = true;

  Serial.println("Firebase initialized.");
}



void sendBinData(int large, int small, int bin, int rewards, int weight, int stock) {
  object_t json, o1, o2, o3, o4, o5, o6, final;
  JsonWriter writer;

  writer.create(o1, "bottle_large", large);
  writer.create(o2, "bottle_small", small);
  writer.create(o3, "bin_level", bin);
  writer.create(o4, "total_rewards", rewards);
  writer.create(o5, "total_weight", weight);
  writer.create(o6, "coin_stock", stock);
  writer.join(final, 6, o1, o2, o3, o4, o5, o6);
  writer.create(json, "/bin", final);

  Serial.println("Sending bin data...");
  Database.update<object_t>(aClient, "/plastech", json, databaseResult);
}

void sendAlertMessage() {
  Messages::Message msg;
  msg.token(DEVICE_REGISTRATION_ID_TOKEN);

  Messages::Notification notif;
  notif.title("PlasTech Alert");
  notif.body("Collection bin nearing full capacity.");
  msg.notification(notif);

  object_t data;
  JsonWriter writer;
  writer.create(data, "status", "alert");
  writer.create(data, "level", "high");
  writer.create(data, "timestamp", millis());
  msg.data(data);

  Messages::AndroidConfig androidCfg;
  Messages::AndroidNotification androidNotif;
  androidNotif.notification_priority(Messages::NotificationPriority::PRIORITY_HIGH);
  androidCfg.notification(androidNotif);
  msg.android(androidCfg);

  Serial.println("Sending FCM message...");
  messaging.send(aClient, Messages::Parent(FIREBASE_PROJECT_ID), msg, fcmResult);
}

void processData(AsyncResult &res) {
  if (!res.isResult()) return;

  if (res.isEvent())
    Firebase.printf("Event [%s]: %s (code: %d)\n", res.uid().c_str(), res.eventLog().message().c_str(), res.eventLog().code());

  if (res.isDebug())
    Firebase.printf("Debug [%s]: %s\n", res.uid().c_str(), res.debug().c_str());

  if (res.isError())
    Firebase.printf("Error [%s]: %s (code: %d)\n", res.uid().c_str(), res.error().message().c_str(), res.error().code());

  if (res.available())
    Firebase.printf("Response [%s]: %s\n", res.uid().c_str(), res.c_str());
}