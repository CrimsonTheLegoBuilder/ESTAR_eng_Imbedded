#ifndef _LEDWAVY_SOURCE_
#define _LEDWAVY_SOURCE_

#include "ledWavy.h"

/*
G - 0 ~ 4
R - 5 ~ 9
B - 10 ~ 14
*/
int NUMPIXELS = 0; // number of neopixels in strip
int INTERVAL = 10;
int CNT = 20;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rlight = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel llight = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led_setup(int x) {
  NUMPIXELS = x; // number of neopixels in strip
  pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  pixels.begin();
}
int set_brightness(int j, int x, int b, int interval) {
  int ipp = (x - 2 + NUMPIXELS * 3) % (NUMPIXELS * 3);
  int ip = (x - 1 + NUMPIXELS * 3) % (NUMPIXELS * 3);
  int i0 = x;
  int in = (x + 1) % (NUMPIXELS * 3);
  int inn = (x + 2) % (NUMPIXELS * 3);
  if ((i0 % interval) == (j % interval)) return 255 - (b * 10);
  if ((ip % interval) == (j % interval)) return 50 + (b * 5);
  if ((in % interval) == (j % interval)) return 150 - (b * 5);
  if ((ipp % interval) == (j % interval)) return 10 + (b * 2);
  if ((inn % interval) == (j % interval)) return 50 - (b * 2);
  else return 0;
}
void wavy(int x) {
  for (int j = 0; j < NUMPIXELS; j++) {
    int g = j * 3 + 0, r = j * 3 + 1, b = j * 3 + 2;
    int G = set_brightness(g, x, 0);
    int R = set_brightness(r, x, 0);
    int B = set_brightness(b, x, 0);
    Serial.print(G);
    Serial.print(" ");
    Serial.print(R);
    Serial.print(" ");
    Serial.print(B);
    Serial.print(" ");
    pixels.setPixelColor(j, pixels.Color(R, G, B));
  }
    Serial.println(" ");
  pixels.show();
}
void wavy_analog(int x) {
  for (int i = CNT; i > 0; i--) {
    for (int j = 0; j < NUMPIXELS; j++) {
      int g = j * 3 + 0, r = j * 3 + 1, b = j * 3 + 2;
      int G = set_brightness(g, x, i);
      int R = set_brightness(r, x, i);
      int B = set_brightness(b, x, i);
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
  }
}
void wavy() {
  static int J_ = 0;
  // Serial.print(J_);
  // Serial.print(" ");
  wavy_analog(J_ + 2);
  // J_ = (J_ + 1) % (NUMPIXELS * 3);
  J_ = (J_ + 1) % INTERVAL;
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

#endif