#ifndef _LEDWAVY_H_
#define _LEDWAVY_H_

#include <Adafruit_NeoPixel.h> 
#define PIN 2 // input pin Neopixel is attached to
#define RF_LIGHT 3 // input pin Neopixel is attached to
#define RR_LIGHT 4 // input pin Neopixel is attached to
#define LF_LIGHT 5 // input pin Neopixel is attached to
#define LR_LIGHT 6 // input pin Neopixel is attached to
  
void led_setup(int x);
int set_brightness(int j, int x, int b, int interval = 12);
void wavy(int x);
void wavy();
void wavy_analog(int x);

#endif