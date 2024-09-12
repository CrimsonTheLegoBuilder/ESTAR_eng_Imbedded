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
  for (int i=0; i < NUMPIXELS; i++) {  
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255  
    setColor(i);  
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));  
    // This sends the updated pixel color to the hardware.  
    pixels.show();  
    // Delay for a period of time (in milliseconds).  
  }
  r = (r + 1) % NUMPIXELS;
  delay(delayval);
  Serial.print(redColor);
  Serial.print(" ");
  Serial.print(greenColor);
  Serial.print(" ");
  Serial.print(blueColor);
  Serial.println(" ");
}  
// setColor()  
// picks random values to set for RGB  
void setColor(int i){  
  redColor = 30 * ((i + r) % NUMPIXELS);
  greenColor = 0;
  // greenColor = 30 * ((i + r) % NUMPIXELS);  
  blueColor = 0;
  // blueColor = 30 * ((i + r) % NUMPIXELS); 
}  