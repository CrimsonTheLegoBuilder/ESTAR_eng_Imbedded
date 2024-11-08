#ifndef _LEDWAVY_H_
#define _LEDWAVY_H_

//#include <Adafruit_NeoPixel.h> 
#include <FastLED.h>

#define LF 26 // input pin Neopixel is attached to
#define LR 27 // input pin Neopixel is attached to
#define RF 33 // input pin Neopixel is attached to
#define RR 25 // input pin Neopixel is attached to
#define NLF 6
#define NLR 6
#define NRF 6
#define NRR 6
#define NUM_LEDS 10

#define RIGHT 1
#define LEFT 2

static CRGB L_F[NUM_LEDS];
static CRGB L_R[NUM_LEDS];
static CRGB R_F[NUM_LEDS];
static CRGB R_R[NUM_LEDS];

struct Strip {
  int f, r;
  int nf, nr;
  // Adafruit_NeoPixel F, R;
  int cnt, interval, pt;
  void led_setup();
  void on_(int n);
  void blink();
  uint8_t set_brightness(int j, int x, int b);
  void wavy_analog(int x);
  void wavy_analog_dir(int x, int dir);
  void wavy(int dir);
  void direction_signal_dir(int x, int dir);
  void direction_signal(int dir);
  void run(int m);
};

#endif