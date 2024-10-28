// Techatronic.com  
#include <Adafruit_NeoPixel.h>  
#include "ledWavy.h"

void setup() {  
  // Initialize the NeoPixel library.
  Serial.begin(9600);
  led_setup(6);
}

void loop() {  
  // wavy();
  on_();
  delay(100);
}  

