#ifndef _LEDWAVY_H_
#define _LEDWAVY_H_

//#include <Adafruit_NeoPixel.h> 
#include <FastLED.h>

#define LF 26 // input pin Neopixel is attached to
#define LR 27 // input pin Neopixel is attached to
#define NLF 6
#define NLR 1
#define NUM_LEDS 10

static CRGB L_F[NUM_LEDS];  // 첫 번째 스트립의 LED 배열
static CRGB L_R[NUM_LEDS];  // 두 번째 스트립의 LED 배열

struct Strip {
  int f, r;
  int nf, nr;
  // Adafruit_NeoPixel F, R;
  int cnt, interval, pt;
  void led_setup();
  void on_();
  uint8_t set_brightness(int j, int x, int b);
  void wavy_analog(int x);
  void wavy();
};

#endif