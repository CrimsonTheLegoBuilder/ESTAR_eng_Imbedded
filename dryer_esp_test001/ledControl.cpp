#ifndef _LED_CONTROL_SOURCE_
#define _LED_CONTROL_SOURCE_

#include "ledControl.h"
#include <FastLED.h>

CRGB leds[50];

int RGB_NUM;
int color = 0;
int rain_pt;

void rgb::init(int rgb_num) {
  RGB_NUM = rgb_num;
  rain_pt = 0;
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, 50);
  return;
}

void rgb::fixed(const int& color) {
  for (int i = 0; i < RGB_NUM; i++) {
    if (color == 1) leds[i] = RED_COLOR;
    else if (color == 2) leds[i] = ORANGE_COLOR;
    else if (color == 3) leds[i] = YELLOW_COLOR;
    else if (color == 4) leds[i] = GREEN_COLOR;
    else if (color == 5) leds[i] = BLUE_COLOR;
    else leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
  return;
}

void rgb::rainbow() {
  rain_pt = (rain_pt + 1) % RGB_NUM;
  for (int i = 0; i < RGB_NUM; i++) {
    int pt = (i + rain_pt) % 25;
    if (0 <= pt && pt < 5) leds[i] = RED_COLOR;
    if (5 <= pt && pt < 10) leds[i] = ORANGE_COLOR;
    if (10 <= pt && pt < 15) leds[i] = YELLOW_COLOR;
    if (15 <= pt && pt < 20) leds[i] = GREEN_COLOR;
    if (20 <= pt && pt < 25) leds[i] = BLUE_COLOR;
  }
  FastLED.show();
  return;
}

void rgb::rotate(const int& color) {
  rain_pt = (rain_pt + 1) % RGB_NUM;
  for (int i = 0; i < RGB_NUM; i++) {
    int pt = (i + rain_pt) % 47;
    if (0 <= pt && pt < 20) {
      if (color == 1) leds[i] = RED_COLOR;
      else if (color == 2) leds[i] = ORANGE_COLOR;
      else if (color == 3) leds[i] = YELLOW_COLOR;
      else if (color == 4) leds[i] = GREEN_COLOR;
      else if (color == 5) leds[i] = BLUE_COLOR;
      else leds[i] = CRGB(255, 255, 255);
    }
    else leds[i] = CRGB(100, 100, 100);
  }
  FastLED.show();
  return;
}

#endif