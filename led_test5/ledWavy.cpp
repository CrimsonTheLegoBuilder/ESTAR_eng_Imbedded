#ifndef _LEDWAVY_SOURCE_
#define _LEDWAVY_SOURCE_

#include "ledWavy.h"

void Strip::led_setup() {
  FastLED.addLeds<NEOPIXEL, LF>(L_F, NLF); // 첫 번째 스트립 초기화
  FastLED.addLeds<NEOPIXEL, LR>(L_R, NLR); // 두 번째 스트립 초기화
  nf = NLF; 
  nr = NLR;
  cnt = 20;
  interval = 12;
  pt = 0;
  FastLED.setBrightness(255);
}
void Strip::on_() {
  for (int i = 0; i < NLF; i++) {
    L_F[i] = CRGB::White;  // 예: 모두 빨간색
  }
  for (int i = 0; i < NLR; i++) {
    L_R[i] = CRGB::White; // 예: 모두 파란색
  }
  FastLED.show();
}
uint8_t Strip::set_brightness(int j, int x, int b) {
  if (j % interval == x % interval) return 155 + (b * 5);
  if ((j + 1) % interval == x % interval) return 90 + (b * 3);
  if ((j + 2) % interval == x % interval) return 40 + (b * 2);
  if ((j - 1 + interval) % interval == x % interval) return 255 - (b * 5);
  if ((j - 2 + interval) % interval == x % interval) return 150 - (b * 3);
  if ((j - 3 + interval) % interval == x % interval) return 80 - (b * 2);
  return 20;
}
void Strip::wavy_analog(int x) {
  int num_pix = nf + nr;
  for (int i = cnt; i > 0; i--) {
    for (int j = 0; j < num_pix; j++) {
      int r = j * 3 + 1, g = j * 3 + 0, b = j * 3 + 2;
      int rr = set_brightness(r, x, i);
      int gg = set_brightness(g, x, i);
      int bb = set_brightness(b, x, i);
      //Serial.print(rr);
      //Serial.print(" ");
      //Serial.print(gg);
      //Serial.print(" ");
      //Serial.print(bb);
      //Serial.print(" ");
      if (j < nf) L_F[j] = CRGB(rr, gg, bb);
      else L_R[num_pix - j - 1] = CRGB(bb, gg, rr);
    }
    //Serial.println(" ");
    FastLED.show();
    // delay(5);
  }
}
void Strip::wavy() {
  // Serial.println(" ");
  // Serial.print(J_);
  // Serial.println(" ");
  wavy_analog(pt);
  pt = (pt + 1) % interval;
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