#ifndef _LEDWAVY_H_
#define _LEDWAVY_H_

#include <Adafruit_NeoPixel.h> 
#define PIN 2 // input pin Neopixel is attached to
#define LF 26 // input pin Neopixel is attached to
#define LR 27 // input pin Neopixel is attached to
  
void led_setup(int x);
uint8_t set_brightness(int j, int x, int b);
// void wavy();
void wavy_analog(int x);
void on_();

struct Strip {
  int f, r;
  int nf, nr;
  Adafruit_NeoPixel F, R;
  int cnt, interval, pt;
  void led_setup(int f_, int nf_, int r_, int nr_);
  void on_();
  uint8_t set_brightness(int j, int x, int b);
  void wavy_analog(int x);
  void wavy();
};

#endif