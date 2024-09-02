#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define FIREBASE_HOST "esp32lidar-default-rtdb.asia-southeast1.firebasedatabase.app"   // Your Firebase Realtime Database URL
#define FIREBASE_AUTH "your-firebase-database-secret"    // Your Firebase Database Secret Key

// Replace with your network credentials
#define WIFI_SSID "est_5G"
#define WIFI_PASSWORD "25142514"

// Define the Lidar sensor serial port and the baud rate
HardwareSerial LidarSerial(2);
#define LIDAR_BAUD_RATE 115200

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
String path = "/LidarData";

void setup() {
  // Start Serial communication with ESP32
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Start Serial communication with Lidar sensor
  LidarSerial.begin(LIDAR_BAUD_RATE, SERIAL_8N1, 16, 17);
}

void loop() {
  if (LidarSerial.available() > 0) {
    // Read the Lidar sensor data
    String lidarData = LidarSerial.readString();

    // Parse and send the data to Firebase
    if (Firebase.RTDB.pushString(&firebaseData, path.c_str(), lidarData)) {
      Serial.println("Data sent to Firebase");
    } else {
      Serial.println("Failed to send data to Firebase");
      Serial.println(firebaseData.errorReason());
    }
  }
  delay(500); // Adjust this delay based on your data rate requirements
}
