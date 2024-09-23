// Techatronic.com  
#include <Adafruit_NeoPixel.h>  
#define PIN 2 // input pin Neopixel is attached to  
#define NUMPIXELS 6 // number of neopixels in strip  
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  
int delayval = 100; // timing delay in milliseconds  
int redColor = 0;  
int greenColor = 0;  
int blueColor = 0;  

int r;

void setup() {  
  // Initialize the NeoPixel library.
  Serial.begin(9600);
  pixels.begin();
  r = 0;
}

void loop() {  
  // for (int i = 0; i < NUMPIXELS; i++) {  
  //   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255  
  //   setColor(i);  
  //   pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));  
  //   // This sends the updated pixel color to the hardware.  
  //   pixels.show();  
  //   // Delay for a period of time (in milliseconds).  
  // }
  // r = (r + 1) % NUMPIXELS;
  // delay(delayval);
  // Serial.print(redColor);
  // Serial.print(" ");
  // Serial.print(greenColor);
  // Serial.print(" ");
  // Serial.print(blueColor);
  // Serial.println(" ");
  wave();
}  

/*
G - 0 ~ 4
R - 5 ~ 9
B - 10 ~ 14
*/

// setColor()  
// picks random values to set for RGB  
void setColor(int i){  
  redColor = 0;
  // redColor = 30 * ((i + r) % NUMPIXELS);
  greenColor = 0;
  // greenColor = 30 * ((i + r) % NUMPIXELS);  
  // blueColor = 0;
  blueColor = 30 * ((i + r) % NUMPIXELS); 
}

void wave_seg(int J, int I) {
  for (int j = 0; j < NUMPIXELS; j++) {
    if (j == J) pixels.setPixelColor(j, pixels.Color((I == 1) * 255, (I == 0) * 255, (I == 2) * 255));
    else pixels.setPixelColor(j, pixels.Color(0, 0, 0));
    pixels.show();
  }
  delay(100);
}
void wave() {
  static int J_ = 0;
  for (int i = 0; i < 3; i++) wave_seg(J_, i);
  J_ = (J_ + 1) % NUMPIXELS;
}