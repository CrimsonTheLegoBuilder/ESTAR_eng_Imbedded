#ifndef _LEDWAVY_SOURCE_
#define _LEDWAVY_SOURCE_

#include "ledWavy.h"

/*
G - 0 ~ 4
R - 5 ~ 9
B - 10 ~ 14
*/
int NUMPIXELS = 6; // number of neopixels in strip
int INTERVAL = 12;
int CNT = 20;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel rlight = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel llight = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led_setup(int x) {
  NUMPIXELS = x; // number of neopixels in strip
  pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  pixels.begin();
}

uint8_t set_brightness(int j, int x, int b) {
  if (j % INTERVAL == x % INTERVAL) return 155 + (b * 5);
  if ((j + 1) % INTERVAL == x % INTERVAL) return 90 + (b * 3);
  if ((j + 2) % INTERVAL == x % INTERVAL) return 50 + (b * 2);
  if ((j - 1 + INTERVAL) % INTERVAL == x % INTERVAL) return 255 - (b * 5);
  if ((j - 2 + INTERVAL) % INTERVAL == x % INTERVAL) return 150 - (b * 3);
  if ((j - 3 + INTERVAL) % INTERVAL == x % INTERVAL) return 90 - (b * 2);
  return 45;
}
void wavy_analog(int x) {
  for (int i = CNT; i > 0; i--) {
    for (int j = 0; j < NUMPIXELS; j++) {
      int g = j * 3 + 0, r = j * 3 + 1, b = j * 3 + 2;
      int G = set_brightness(g, x, i);
      int R = set_brightness(r, x, i);
      int B = set_brightness(b, x, i);
      // delay(1000);
      // Serial.print(G);
      // Serial.print(" ");
      // Serial.print(R);
      // Serial.print(" ");
      // Serial.print(B);
      // Serial.print(" ");
      pixels.setPixelColor(j, pixels.Color(R, G, B));
    }
    // Serial.println(" ");
    pixels.show();
    delay(20);
  }
}
void wavy() {
  static int J_ = 0;
  Serial.println(" ");
  Serial.print(J_);
  Serial.println(" ");
  wavy_analog(J_);
  // J_ = (J_ + 1) % (NUMPIXELS * 3);
  J_ = (J_ + 1) % INTERVAL;
}

void on_() {
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels.setPixelColor(j, pixels.Color(255, 255, 255));
  }
  Serial.println(" ");
  pixels.show();
}

// void wave_seg(int J, int I) {
//   for (int j = 0; j < NUMPIXELS; j++) {
//     if (j == J) pixels.setPixelColor(j, pixels.Color((I == 1) * 255, (I == 0) * 255, (I == 2) * 255));
//     else pixels.setPixelColor(j, pixels.Color(0, 0, 0));
//     pixels.show();
//   }
//   delay(100);
// }
// void wave() {
//   static int J_ = 0;
//   for (int i = 0; i < 3; i++) wave_seg(J_, i);
//   J_ = (J_ + 1) % NUMPIXELS;
// }
// uint8_t set_brightness(int j, int x, int b) {
//   int ipp = (j - 2 + NUMPIXELS * 3) % (NUMPIXELS * 3);
//   int ip = (j - 1 + NUMPIXELS * 3) % (NUMPIXELS * 3);
//   int i0 = j;
//   int in = (j + 1) % (NUMPIXELS * 3);
//   int inn = (j + 2) % (NUMPIXELS * 3);
//   // Serial.print(ipp);
//   // Serial.print(" ");
//   // Serial.print(ip);
//   // Serial.print(" ");
//   // Serial.print(i0);
//   // Serial.print(" ");
//   // Serial.print(in);
//   // Serial.print(" ");
//   // Serial.print(inn);
//   // Serial.print(" ");
//   // Serial.print(ipp % INTERVAL);
//   // Serial.print(" ");
//   // Serial.print(ip % INTERVAL);
//   // Serial.print(" ");
//   // Serial.print(i0 % INTERVAL);
//   // Serial.print(" ");
//   // Serial.print(in % INTERVAL);
//   // Serial.print(" ");
//   // Serial.print(inn % INTERVAL);
//   // Serial.println(" ");
//   if ((i0 % INTERVAL) == x) return 255 - (b * 10);
//   if ((ip % INTERVAL) == x) return 50 + (b * 5);
//   if ((in % INTERVAL) == x) return 150 - (b * 5);
//   if ((ipp % INTERVAL) == x) return 10 + (b * 2);
//   if ((inn % INTERVAL) == x) return 50 - (b * 2);
//   else return 0;
// }
#endif