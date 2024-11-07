#ifndef _LEDWAVY_SOURCE_
#define _LEDWAVY_SOURCE_

#include "ledWavy.h"

void Strip::led_setup() {
  FastLED.addLeds<NEOPIXEL, LF>(L_F, NLF);
  FastLED.addLeds<NEOPIXEL, LR>(L_R, NLR);
  FastLED.addLeds<NEOPIXEL, RF>(R_F, NRF);
  FastLED.addLeds<NEOPIXEL, RR>(R_R, NRR);
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
      else L_R[num_pix - j - 1] = CRGB(rr, bb, gg);
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

#endif