#include <Adafruit_NeoPixel.h>


#define LED_PIN 3
#define LED_COUNT 6

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();            // 네오픽셀 시작
  strip.show();             // 네오픽셀 출력
  strip.setBrightness(25);  // 밝기 설정 0 ~ 255

}

void loop() {

 /* //  colorWipe(strip.Color(R, G, B), Time);
  colorWipe(strip.Color(151, 0, 0), 100);      // Red
  colorWipe(strip.Color(101, 255, 25), 100);   // Green
  colorWipe(strip.Color(0, 154, 138), 100);    // Blue
  //colorWipe(strip.Color(255, 255, 255), 100); // White*/




  // 극장간판에 달린 전구가 빛나는것과 유사한 효과
  // theaterChase(스트립 색상, 딜레이 시간)
  //theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  //theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  //theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

  // 전체 스트립에 색을 흐르는 무지개빛처럼 돌아가며 출력
  rainbow(10);           
  // 위에 theaterChase효과를 무지개빛으로 출력
  //theaterChaseRainbow(50); 
  
  //해당 함수들은 밑에 구현되어있다.
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) {   // Repeat 10 times...
    for (int b = 0; b < 3; b++) {  //  'b' counts from 0 to 2...
      strip.clear();               //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);  // Set pixel 'c' to value 'color'
      }
      strip.show();  // Update strip with new contents
      delay(wait);   // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;
  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 3; b++) {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
        int hue = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue));  // hue -> RGB
        strip.setPixelColor(c, color);
      }
      strip.show();
      delay(wait);
      firstPixelHue += 65536 / 90;
    }
  }
}