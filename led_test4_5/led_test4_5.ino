#include <FastLED.h>

#define LED_PIN_F 26  // 첫 번째 스트립의 핀 번호
#define LED_PIN_R 27  // 두 번째 스트립의 핀 번호
#define NUM_LEDS_F 1  // 첫 번째 스트립의 LED 개수
#define NUM_LEDS_R 1  // 두 번째 스트립의 LED 개수
#define BRIGHTNESS  64

CRGB leds_F[NUM_LEDS_F];  // 첫 번째 스트립의 LED 배열
CRGB leds_R[NUM_LEDS_R];  // 두 번째 스트립의 LED 배열

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN_F>(leds_F, NUM_LEDS_F); // 첫 번째 스트립 초기화
  FastLED.addLeds<NEOPIXEL, LED_PIN_R>(leds_R, NUM_LEDS_R); // 두 번째 스트립 초기화
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // 첫 번째 스트립에 패턴 적용
  for (int i = 0; i < NUM_LEDS_F; i++) {
    leds_F[i] = CRGB::Red;  // 예: 모두 빨간색
  }

  // 두 번째 스트립에 다른 패턴 적용
  for (int i = 0; i < NUM_LEDS_R; i++) {
    leds_R[i] = CRGB::Blue; // 예: 모두 파란색
  }

  FastLED.show();  // 모든 스트립에 변경 사항 적용
  //delay(1000);     // 딜레이 (변경 간격 조정 가능)

  // 모든 LED 끄기
  //fill_solid(leds_F, NUM_LEDS_F, CRGB::Black);
  //fill_solid(leds_R, NUM_LEDS_R, CRGB::Black);
  //FastLED.show();
  //delay(1000);
}