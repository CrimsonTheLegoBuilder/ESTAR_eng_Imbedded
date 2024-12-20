#ifndef _LED_CONTROL_SOURCE_
#define _LED_CONTROL_SOURCE_

#include "ledControl.h"
#include <FastLED.h>

CRGB leds[50];

int RGB_NUM;
int color = 0;

void rgb::init(int rgb_num) {
  RGB_NUM = rgb_num;
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, 50);
  return;
}

void rgb::fixed(int color) {
  for (int i = 0; i < RGB_NUM; i++) {
    if (color == 1) leds[i] = RED_COLOR;
    else if (color == 2) leds[i] = ORANGE_COLOR;
    else if (color == 3) leds[i] = YELLOW_COLOR;
    else if (color == 4) leds[i] = GREEN_COLOR;
    else if (color == 5) leds[i] = BLUE_COLOR;
    else leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
  delay(100);
  return;
}

#endif