/**
* Created by K. Suwatchai (Mobizt)
*
* Email: k_suwatchai@hotmail.com
*
* Github: https://github.com/mobizt/Firebase-ESP8266
*
* Copyright (c) 2023 mobizt
*
*/

/** This example will show how to authenticate as a user with Email and password.
*
* You need to enable Email/Password provider.
* In Firebase console, select Authentication, select Sign-in method tab,
* under the Sign-in providers list, enable Email/Password provider.
*
* From this example, the user will be granted to access the specific location that matches
* the user uid.
*
* This example will modify the database rules to set up the security rule which need to
* guard the unauthorized access with the user Email.
*/

//#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
// #define WIFI_SSID "U+Net3B80"
// #define WIFI_PASSWORD "gywlsl12#$"
// String ssid = "U+Net3B80";
// String password = "gywlsl12#$";
#define WIFI_SSID "est"
#define WIFI_PASSWORD "25142514"
String ssid = "est";
String password = "25142514";
/** 2. Define the API key
*
* The API key (required) can be obtained since you created the project and set up
* the Authentication in Firebase console. Then you will get the API key from
* Firebase project Web API key in Project settings, on General tab should show the
* Web API Key.
*
* You may need to enable the Identity provider at https://console.cloud.google.com/customer-identity/providers
* Select your project, click at ENABLE IDENTITY PLATFORM button.
* The API key also available by click at the link APPLICATION SETUP DETAILS.
*
*/
#define API_KEY "AIzaSyCNok7I9aeKuF8G8hesmel2bN8KONlYEEE"

/* 3. Define the user Email and password that already registerd or added in your project */
#define USER_EMAIL "jjacko3@hotmail.com"
#define USER_PASSWORD "111111"
String strFirebasePath = "";// "/users/EyhMDd7BUDeglqBLKFp2IWpyLC52/places/8ebc32d0-bb28-1d35-a564-3f24479bd155/devices/afee6cc0-3f57-1e59-941a-219cb621a487";
String userEmail= "jjacko3@hotmail.com";
String userPass= "111111";
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
int btnGPIO = 0;
int btnState = false;
bool bFirebaseCallReady = false;
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
int nStep = 0; // 0: init 1: wifi con 2: firebase con
/* 4. If work with RTDB, define the RTDB URL */
#define DATABASE_URL "https://eastarsmart-bd545-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
void firebaseSetup()
{
Serial.println("firebaseSetup 1 ");
config.api_key = API_KEY;
config.database_url = DATABASE_URL;
config.token_status_callback = tokenStatusCallback;
Firebase.reconnectNetwork(true);
fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
fbdo.setResponseSize(4096);
auth.user.email = userEmail;
auth.user.password = userPass;
Serial.println(userEmail);
Serial.println(userPass);
Firebase.begin(&config, &auth);
Firebase.setDoubleDigits(5);
nStep++;
Serial.println("firebaseSetup 5 ");
}
void wifiSetup()
{
Serial.println();
Serial.print("[WiFi] Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(300);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
nStep++;
pCharacteristic->setValue("wificon");
pCharacteristic->notify();
}
class MyServerCallbacks: public BLEServerCallbacks {
void onConnect(BLEServer* pServer) {
deviceConnected = true;
value = 0;
Serial.println("BLEServerCallbacks onconnect");
};

void onDisconnect(BLEServer* pServer) {
deviceConnected = false;
value = 0;
Serial.println("BLEServerCallbacks onDisconnect");
}
};
void parser(String strInput)
{
Serial.print("SWITCH ");
Serial.println(strInput);
if (strInput.startsWith("SSID=")) {
Serial.println("SSID Value");
ssid = strInput;
ssid.remove(0,5);
} else if (strInput.startsWith("PASSWORD=")) {
Serial.println("PASSWORD Value");
password = strInput;
password.remove(0,9);
wifiSetup();
}else if (strInput.startsWith("FIREBASE=")) {
Serial.println("FIREBASE Value");
strFirebasePath = strInput;
strFirebasePath.remove(0,9);
//firebaseSetup();
}else if (strInput.startsWith("userEmail=")) {
Serial.println("userEmail Value");
userEmail = strInput;
userEmail.remove(0,10);
//firebaseSetup();
}else if (strInput.startsWith("userPass=")) {
Serial.println("userPass Value");
userPass = strInput;
userPass.remove(0,9);
bFirebaseCallReady = true;
}
else if (strInput.startsWith("SCAN")) {
Serial.println(F("Scanning wifi visible"));
int n = WiFi.scanNetworks();
} else if (strInput.startsWith("CONNECT")) {
}

}
// 앱으로 부터 데이터를 받는 부분
class MyCallbacks: public BLECharacteristicCallbacks {
void onWrite(BLECharacteristic *pCharacteristic) {
uint8_t * rxValue = pCharacteristic->getData();
uint16_t rxLength = pCharacteristic->getLength();
char * ptr;
int index = 0;
if (rxLength > 0) {

String input = (char*)rxValue;
// 디버깅
Serial.println("*********");
Serial.println("Received Value");
Serial.print("\tHEX: ");
Serial.println(input);
ptr = strtok((char*)rxValue, "\b"); // takes a list of delimiters
while(ptr != NULL)
{
Serial.print("PARSER ");
Serial.println(ptr);
//strings[index] = ptr;
String inputString(ptr);
parser(inputString);
index++;
ptr = strtok(NULL, "\b"); // takes a list of delimiters
}
}
}
};

void setup()
{
value = 0;
Serial.println("setup");
Serial.begin(115200);

// Create the BLE Device
BLEDevice::init("ESTARSMART AIR");//ESTARSMART 문구 포함시 자동 연결

// Create the BLE Server
pServer = BLEDevice::createServer();
pServer->setCallbacks(new MyServerCallbacks());

// Create the BLE Service
BLEService *pService = pServer->createService(SERVICE_UUID);
// Create a BLE Characteristic
pCharacteristic = pService->createCharacteristic(
CHARACTERISTIC_UUID,
BLECharacteristic::PROPERTY_READ |
BLECharacteristic::PROPERTY_WRITE |
BLECharacteristic::PROPERTY_NOTIFY |
BLECharacteristic::PROPERTY_INDICATE
);

pCharacteristic->setCallbacks(new MyCallbacks());
pCharacteristic->addDescriptor(new BLE2902());
pService->start();
// Start advertising
BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
pAdvertising->addServiceUUID(SERVICE_UUID);
pAdvertising->setScanResponse(false);
pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
BLEDevice::startAdvertising();
Serial.println("Waiting a client connection to notify...");


while(!bFirebaseCallReady)
{
delay(1000);
}
firebaseSetup();
while(nStep < 2)
{
delay(1000);
}
Serial.println("firebase send pre");
pCharacteristic->setValue("firebaseinit");
pCharacteristic->notify();
Serial.println("firebase send");

// wifiSetup();
// firebaseSetup();
}

void loop()
{
if (millis() - dataMillis > 50000 )
{
dataMillis = millis();
if(Firebase.ready())
{
if(count > 2)
return;
String path = "/users/EAmdgEOzqFTC4ZzReJYxZzdHd7A2/places/a38adf10-e3b9-1e5a-9507-a3c89d2d2e87/devices/9b5dc3b0-0b21-1e6d-b7d9-5f3c414d6b0e";
if(strFirebasePath.length() > 0 )
path = strFirebasePath;
//path += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
path += "/co2";
// bool state; 전원
// int temperature; 온도
// int humidity; 습도
// double co_ppm; 일산화탄소
// int voc_ppm; voc
// int co2; 이상화탄소
// double hPa; 압력센서
// int tvoc; tvoc
// int pm2_5; 초미세먼지
// int pm10; 미세먼지
// int fan; 팬
// String laser; 레이저
Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, path, count++) ? "ok" : fbdo.errorReason().c_str());
}
}
}
