#include "ledWavy.h"
#include <ACAN_ESP32.h>

Strip L;
unsigned long t = 0;
int mode;

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
  mode = 0;
}

void loop() {
  //wavy();
  //on_();
  // L.wavy();
  //L.blink();
  // L.on_();
  //delay(100);
  CANMessage frame;
  if (ACAN_ESP32::can.receive(frame)) {
    Serial.print("received ID : ");
    Serial.println(frame.id, HEX);
    switch (frame.id) {
      case 0x400:
        mode = 0;
        break;
      case 0x401:
        mode = 1;
        break;
      case 0x402:
        mode = 2;
        break;
      case 0x403:
        mode = 3;
        break;
      case 0x404:
        mode = 4;
        break;
    }
  }
  //mode = 3;
  L.run(mode);
  //해당 모드로 동작하는 상태 머신이 필요

}  

/*

freeRTOS 로 태스크를 만들면 폴링과 다르게 태스크를 켜고 끄는 것으로 불을 켤 수 있음.

*/


