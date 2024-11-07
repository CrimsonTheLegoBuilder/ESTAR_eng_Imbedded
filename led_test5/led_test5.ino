#include "ledWavy.h"
#include <ACAN_ESP32.h>

Strip L;
unsigned long t = 0;

void setup() {  
  Serial.begin(9600);
  L.led_setup();
  ACAN_ESP32_Settings settings(500000UL);
  settings.mRxPin = GPIO_NUM_19; //TX2
  settings.mTxPin = GPIO_NUM_18; //RX2
  const uint32_t ret = ACAN_ESP32::can.begin(settings);
  if (ret == 0) {    
    Serial.print("Bit Rate prescaler: ") ;
    Serial.println(settings.mBitRatePrescaler) ;
    Serial.print("Time Segment 1:     ") ;
    Serial.println(settings.mTimeSegment1) ;
    Serial.print("Time Segment 2:     ") ;
    Serial.println(settings.mTimeSegment2) ;
    Serial.print("RJW:                ") ;
    Serial.println(settings.mRJW) ;
    Serial.print("Triple Sampling:    ") ;
    Serial.println(settings.mTripleSampling ? "yes" : "no") ;
    Serial.print("Actual bit rate:    ") ;
    Serial.print(settings.actualBitRate ()) ;
    Serial.println(" bit/s") ;
    Serial.print("Exact bit rate ?    ") ;
    Serial.println(settings.exactBitRate () ? "yes" : "no") ;
    Serial.print("Distance            ") ;
    Serial.print(settings.ppmFromDesiredBitRate ()) ;
    Serial.println(" ppm") ;
    Serial.print("Sample point:       ") ;
    Serial.print(settings.samplePointFromBitStart ()) ;
    Serial.println("%") ;
    Serial.println("Configuration OK!");
  }
  else {
    Serial.print("Configuration error 0x") ;
    Serial.println(ret, HEX) ;    
  }
}

void loop() {
  //wavy();
  //on_();
  L.wavy();
  // L.on_();
  //delay(100);
}  

