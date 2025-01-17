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
void Strip::on_(int n) {
  for (int i = 0; i < NLF; i++) {
    if (n) L_F[i] = CRGB::White;
    else L_F[i] = CRGB::Black;
  }
  for (int i = 0; i < NLR; i++) {
    if (n) L_R[i] = CRGB::White;
    else L_R[i] = CRGB::Black;
  }
  for (int i = 0; i < NRF; i++) {
    if (n) R_F[i] = CRGB::White;
    else R_F[i] = CRGB::Black;
  }
  for (int i = 0; i < NRR; i++) {
    if (n) R_R[i] = CRGB::White;
    else R_R[i] = CRGB::Black;
  }
  FastLED.show();
}
void Strip::blink() {
  pt = (pt + 1) % 1000;
  on_(pt > 500);
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
void Strip::wavy_analog_dir(int x, int dir) {
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
      if (dir == LEFT) {
        if (j < nf) L_F[j] = CRGB(rr, gg, bb), R_F[j] = CRGB::Black;
        else L_R[num_pix - j - 1] = CRGB(rr, bb, gg), R_R[num_pix - j - 1] = CRGB::Black;
      }
      else if (dir == RIGHT) {
        if (j < nf) R_F[j] = CRGB(rr, gg, bb), L_F[j] = CRGB::Black;
        else R_R[num_pix - j - 1] = CRGB(rr, bb, gg), L_R[num_pix - j - 1] = CRGB::Black;
      }
    }
    //Serial.println(" ");
    FastLED.show();
    // delay(5);
  }
}
void Strip::wavy(int dir) {
  pt = (pt + 1) % interval;
  wavy_analog(pt);
}
void Strip::direction_signal_dir(int x, int dir) {
  for (int i = cnt; i > 0; i--) {
    for (int j = 0; j < nf; j++) {
      int r = j * 3 + 1, g = j * 3 + 0, b = j * 3 + 2;
      int rr = set_brightness(r, x, i);
      int gg = set_brightness(g, x, i);
      int bb = set_brightness(b, x, i);
      // Serial.print(rr);
      // Serial.print(" ");
      // Serial.print(gg);
      // Serial.print(" ");
      // Serial.print(bb);
      // Serial.print(" ");
      if (dir == LEFT) {
        L_F[j] = L_R[j] = CRGB(rr, gg, bb);
        R_F[j] = R_R[j] = CRGB::Black;
      }
      else if (dir == RIGHT) {
        R_F[j] = R_R[j] = CRGB(rr, gg, bb);
        L_F[j] = L_R[j] = CRGB::Black;
      }
    }
    //Serial.println(" ");
    FastLED.show();
    delay(5);
  }
}
void Strip::direction_signal(int dir) {
  pt = (pt + 1) % interval;
  direction_signal_dir(pt, dir);
}
void Strip::run(int m) {
  switch (m) {
    case 0:
      blink();
      break;
    case 1:
      on_(1);
      break;
    case 2:
      direction_signal(LEFT);
      break;
    case 3:
      direction_signal(RIGHT);
      break;
    case 4:
      on_(0);
      break;
  }
  return;
}





#endif