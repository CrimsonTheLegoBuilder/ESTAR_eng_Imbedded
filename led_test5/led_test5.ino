// Techatronic.com  
//#include <Adafruit_NeoPixel.h>  
#include "ledWavy.h"
//#include "FastLED.h"

Strip L;

void setup() {  
  // Initialize the NeoPixel library.
  Serial.begin(9600);
  //led_setup(6);
  L.led_setup();
}

void loop() {  
  //wavy();
  //on_();
  L.wavy();
  // L.on_();
  //delay(100);
}  

