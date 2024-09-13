#include <FastLED.h>

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
int RGB_NUM;
int rain_pt;

CRGB leds[100];

void init(int rgb_num) {
  RGB_NUM = rgb_num;
  FastLED.addLeds<WS2811, RGB_PIN, GRB>(leds, 47);
  rain_pt = 0;
  return;
}

void fixed(const int& color) {
  for (int i = 0; i < RGB_NUM; i++) {
    if (color == 1) leds[i] = RED_COLOR;
    else if (color == 2) leds[i] = ORANGE_COLOR;
    else if (color == 3) leds[i] = YELLOW_COLOR;
    else if (color == 4) leds[i] = GREEN_COLOR;
    else if (color == 5) leds[i] = BLUE_COLOR;
    else leds[i] = CRGB(237, 230, 219);
  }
  FastLED.show();
  return;
}

void rainbow() {
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

void rotate(int color) {
  rain_pt = (rain_pt + 1) % RGB_NUM;
  for (int i = 0; i < RGB_NUM; i++) {
    int pt = (i + rain_pt) % 2;
    if (0 <= pt && pt < 1) {
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

void setup() {
  init(60);
}

void loop() {
  // 1: red | 2: orange | 3: yellow | 4: green | 5: blue | 0: white
  fixed(0);
  // rainbow();
  // rotate(0);
  // delay(60);
}
