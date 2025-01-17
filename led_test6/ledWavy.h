#ifndef _LEDWAVY_H_
#define _LEDWAVY_H_

//#include <Adafruit_NeoPixel.h> 
#include <FastLED.h>

#define LF 2 // input pin Neopixel is attached to
#define LR 26 // input pin Neopixel is attached to
#define RF 25 // input pin Neopixel is attached to
#define RR 12 // input pin Neopixel is attached to
#define FRONT 32 // input pin Neopixel is attached to
#define REAR 33 // input pin Neopixel is attached to

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

/*

비상등 켜기 끄기
전조등 켜기 끄기 아이디에 대해 켜고 끄고 지시 여부는 내용물에 넣을 수 있음.
후미등 켜기 끄기
지시등 오른쪽 켜기 끄기
지시등 왼쪽 켜기 끄기
별도 패턴?

소리는 어떤 보드에 할당을 해야할까?
상대적으로 시간에 구애를 받지 않는 쪽은 어디인가? 눈으로 보이는 부분은 느려지면 바로 티가 나므로 초음파 쪽에 할당?을 하는 게 맞는 거 같은데...

*/

#endif