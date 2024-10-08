#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

#include <Arduino.h>

#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define BLUE 5

#define RED_COLOR CRGB(255, 0, 0)
#define ORANGE_COLOR CRGB(255, 150, 0)
#define YELLOW_COLOR CRGB(255, 255, 0)
#define GREEN_COLOR CRGB(0, 255, 0)
#define BLUE_COLOR CRGB(0, 0, 255)

#define RGB_PIN 2
extern int RGB_NUM;

namespace rgb {
  void init(int rgb_num);
  void fixed(const int& color);
}

#endif