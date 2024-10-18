//수신기

#include <ACAN_ESP32.h>

void setup() {
  Serial.begin(115200);
  ACAN_ESP32_Settings settings(125000UL);

  //settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode;
  
  //Default Tx:GPIO4, Rx:GPIO5
  settings.mRxPin = GPIO_NUM_18;
  settings.mTxPin = GPIO_NUM_19;

  const uint32_t ret = ACAN_ESP32::can.begin(settings) ;
  
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
  } else {
    Serial.print("Configuration error 0x") ;
    Serial.println(ret, HEX) ;    
  }
}

void loop() {
  CANMessage frame;
  // Serial.println("fuck");
  //캔통신으로 수신된 데이터가 있는가?
  if(ACAN_ESP32::can.receive(frame)) {
    Serial.println("suck");
    //frame.ext : ID종류가 29bit인가? 아니면 11bit인가
    //frame.rtr : 리모트인가? 아니면 데이터인가
    //frame.id : 아이디
    //frame.len : 수신데이터 길이
    //frame.data : 수신데이터(배열)
    if(frame.ext){
      Serial.println("EXTENDED ID입니다!");
    }else{
      Serial.println("STANDARD ID입니다!");
    }

    if(frame.rtr){
      Serial.println("RTR : REMOTE!");
    }else{
      Serial.println("RTR : DATA!");
    }

    Serial.print("수신받은 ID : ");
    Serial.println(frame.id,HEX);

    Serial.print("데이터길이 : ");
    Serial.println(frame.len);

    //결과를 16진수로출력
    Serial.print("데이터를 16진수로 표현 : ");
    for(int i = 0;i<frame.len;i++){
      Serial.print(frame.data[i],HEX);
      Serial.print(", ");
    }
    Serial.println();

    //결과를 아스키코드로출력
    Serial.print("데이터를 16진수로 표현 : ");
    for(int i = 0;i<frame.len;i++){
      Serial.print((char)frame.data[i]);
      Serial.print(", ");
    }
    Serial.println();
  }  
}