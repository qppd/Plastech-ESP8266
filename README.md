<h1 align="center">♻️ Plastech ESP8266 Firmware</h1>

<p align="center">
  <strong>Firmware for the IoT Board supporting the <a href="https://github.com/qppd/PlasTech">PlasTech</a> waste management app.</strong>
  <br><br>
  <img src="https://img.shields.io/badge/Platform-ESP8266-blue.svg" alt="Platform Badge">
  <img src="https://img.shields.io/badge/Language-C++-brightgreen.svg" alt="Language Badge">
  <img src="https://img.shields.io/badge/License-MIT-green.svg" alt="License Badge">
</p>

---

<h2>📡 Overview</h2>

<p>
  <code>Plastech-ESP8266</code> is the embedded firmware for an ESP8266-based IoT board designed to work with the <a href="https://github.com/qppd/PlasTech">PlasTech</a> Android app. This project enables real-time communication between physical waste bins and the mobile app, tracking user recycling actions via sensors and Firebase.
</p>

---

<h2>🛠 Features</h2>

<ul>
  <li>Wi-Fi connectivity using ESP8266</li>
  <li>Real-time updates with Firebase Realtime Database</li>
  <li>Modular and expandable firmware design</li>
</ul>

---

<h2>⚙️ Setup & Installation</h2>

<ol>
  <li>Clone this repository:</li>
  <pre><code>git clone https://github.com/qppd/Plastech-ESP8266.git</code></pre>

  <li>Open the project using Arduino IDE.</li>

  <li>Edit code with your Wi-Fi and Firebase credentials.</li>

  <pre><code>
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define DATABASE_URL ""
#define FIREBASE_PROJECT_ID ""
#define FIREBASE_CLIENT_EMAIL ""
const char PRIVATE_KEY[] PROGMEM = "";
#define USER_EMAIL ""
#define USER_PASSWORD ""
#define DEVICE_REGISTRATION_ID_TOKEN ""
  </code></pre>

  <li>Connect your ESP8266 device (e.g. NodeMCU, Wemos D1 Mini) via USB.</li>

  <li>Upload the firmware and open the serial monitor to check output.</li>
</ol>

---

<h2>📲 Integration with PlasTech App</h2>

<p>
  This firmware is made to work seamlessly with the <a href="https://github.com/qppd/PlasTech">PlasTech</a> Android app. When users deposit plastic waste into smart bins, the ESP8266 sends data to Firebase, which is then displayed in the mobile app to track device data.
</p>

---

<h2>🧪 Hardware Requirements</h2>

<ul>
  <li>ESP8266 development board (e.g., NodeMCU)</li>
  <li>Wi-Fi connectivity with internet access</li>
  <li>Power source (USB or battery bank)</li>
</ul>

---

<h2>🔐 Firebase Notes</h2>

<p>
  Make sure to set up your Firebase Realtime Database with public read/write access for testing (or configure proper rules for security in production). Example database rules:
</p>

<pre><code>
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
</code></pre>

<p>
  For production, generate a proper token or use Firebase Authentication to secure access.
</p>

---

<h2>🚀 Example Use Case</h2>

<ol>
  <li>ESP8266 sends device data to Firebase</li>
  <li>Mobile app displays the updated points/rewards</li>
</ol>

---

<h2>📌 Roadmap</h2>

<ul>
  <li>[ ] Add Over-The-Air (OTA) firmware updates</li>
</ul>

---

<h2>🧑‍💻 Contributions</h2>

<p>
  Contributions are welcome! Please fork the repo and submit pull requests for enhancements, bug fixes, or new hardware integrations.
</p>

---

<h2>📝 License</h2>

<p>
  This project is licensed under the <strong>MIT License</strong> — see the <a href="LICENSE">LICENSE</a> file for details.
</p>

---

<h2>🙌 Acknowledgments</h2>

<p>
  Developed by <strong>QPPD</strong> as part of the <a href="https://github.com/qppd/PlasTech">PlasTech</a> initiative for smart and sustainable recycling.
</p>
