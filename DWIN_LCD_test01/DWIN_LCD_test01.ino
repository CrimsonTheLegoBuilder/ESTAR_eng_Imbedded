#include <DHT.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <FastLED.h>
#define RED_COLOR CRGB(255, 0, 0)
#define YELLOW_COLOR CRGB(255, 255, 0)
#define ORANGE_COLOR CRGB(255, 150, 0)
#define GREEN_COLOR CRGB(0, 255, 0)
#define BLUE_COLOR CRGB(0, 0, 255)
const int LEN = 9;
const int NUM_LEDS = 27;
const byte rx = 9;
const byte tx = 10;
const int light = 6;
const int light2 = 3;
const int trig = 12;
const int echo = 11;

SoftwareSerial mySerial(rx, tx);

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
#define temperature_add 0x61
#define humidity_add 0x62
#define DHTPIN 7
#define DHTTYPE DHT11
#define LED_PIN 2
DHT dht(DHTPIN, DHTTYPE);
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

CRGB leds[NUM_LEDS];

void setup() {
  dht.begin();
  Serial.begin(115200);
  mySerial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(light, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(light, LOW);
  analogWrite(light2, 0);
}

void temphumicheck() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);

  Temperature[6] = highByte(t);
  Temperature[7] = lowByte(t);
  mySerial.write(Temperature, 8);
  // for (int i = 0; i <= LEN - 2; i++) {
  //   Serial.print(Temperature[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();

  Humidity[6] = highByte(h);
  Humidity[7] = lowByte(h);
  mySerial.write(Humidity, 8);
  // for (int i = 0; i <= LEN - 2; i++) {
  //   Serial.print(Humidity[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();
}

void relay_control() {
  if (mySerial.available() > 7) {
    for (int i = 0; i <= LEN; i++) Buffer[i] = mySerial.read();
    for (int i = 0; i <= LEN; i++) {
      Serial.print(Buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    if (Buffer[0] == 0X5A && Buffer[4] == 0X56) {
      Serial.println(Buffer[8], HEX);
      if (Buffer[8] == 1) digitalWrite(light, HIGH);
      else digitalWrite(light, LOW);
    }
    if (Buffer[0] == 0X5A && Buffer[4] == 0X55) {
      int brightness = Buffer[8];
      Serial.println(brightness);
      analogWrite(light2, brightness);
    }
  }
}

void sonic() {
  float length, distance;
  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);

  length = pulseIn(echo, HIGH);

  distance = ((float)(340 * length) / 10000) / 2;/

  if (distance > 30) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = BLUE_COLOR;
      FastLED.show();
    }
  } else if (distance > 20) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = GREEN_COLOR;
      FastLED.show();
    }
  } else if (distance > 10) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = YELLOW_COLOR;
      FastLED.show();
    }
  } else if (distance > 5) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = RED_COLOR;
      FastLED.show();
    }
  }
}

void loop() {
  temphumicheck();
  delay(200);
  relay_control();
  delay(500);
  sonic();
}