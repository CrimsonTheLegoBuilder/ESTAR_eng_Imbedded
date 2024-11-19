#include "sona.h"
#include <ACAN_ESP32.h>

const int trigPin[3] = { T1, T2, T3 };
const int echoPin[11] = { E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11 };
const uint32_t canID[11] = { I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11 };
//float dist[11];
//uint8_t SIG[15];

float detect(int trig, int echo, int f = 0) {
  if (!f) { digitalWrite(trig, 0); return -1; }
  long duration, distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, TIMEOUT);
  distance = duration * 17 / 1000; 
  delayMicroseconds(40);
  return distance;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("fuck");
  pinMode(trigPin[0], OUTPUT);
  pinMode(trigPin[1], OUTPUT);
  pinMode(trigPin[2], OUTPUT);
  for (int i = 0; i < 11; i++) {
    pinMode(echoPin[i], INPUT);
  }
  //pinMode(RXpin, OUTPUT);
  //pinMode(TXpin, INPUT);

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
  } else {
    Serial.print("Configuration error 0x") ;
    Serial.println(ret, HEX) ;    
  }
}

void loop() {
  CANMessage frame;
  // put your main code here, to run repeatedly:
  // for (int i = 0; i < 11; i++) {
  //   float d = detect(pinNum[i]);
  // }
#ifdef DEBUG
  for (int i = 0; i < 7; i++) {
    float d = detect(trigPin[0], echoPin[i], 1);
    Serial.print("dist:: [");
    Serial.print(i);
    Serial.print("]:: ");
    Serial.println(d);
    delay(40);
  }
#else
  for (int i = 0; i < 11; i++) {
    int trig = i < 4 ? trigPin[0] : i < 7 ? trigPin[1] : trigPin[2]; 
    float d = detect(trig, echoPin[i], 1);
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(d);
    
    //호출하고자하는 슬레이브의 ID
    //standard일때(11bit)
    //0x000(0) ~ 7FF(2,047)
    //extended일때(29bit)
    //0x000(0) ~ 0x1FFFFFFF(536,870,911)
    frame.id = canID[i];
    frame.rtr = 0;
    frame.len = 8;

    frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    frame.data[2] = 0x00;
    frame.data[3] = 0x00;
    frame.data[4] = 0x00;
    frame.data[5] = 0x00;
    frame.data[6] = 0x00;
    frame.data[7] = 0x00;

    //전송
    if (ACAN_ESP32::can.tryToSend(frame)) {
      //성공 
      Serial.println("전송에 성공했습니다");
    }   
    delay(20);
  }
  delay(1000);
#endif
}
