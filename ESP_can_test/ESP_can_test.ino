// ESP32 CAN 라이브러리 헤더
#include <ACAN_ESP32.h>

void setup() {
  // 모니터 프로그램용 시리얼
  Serial.begin(115200);
  
  // 설정 변수 선언(통신 속도 설정)
  ACAN_ESP32_Settings settings(125000UL); //125Kbps

  // Loopback 모드 테스트시에는 활성, 여기선 다른 기기와 테스트 할 것이므로 제거
  //settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode;
  
  // 핀 설정을 따로 하지 않는다면 Tx는 GPIO4, Rx는 GPIO5이 된다.
  settings.mRxPin = GPIO_NUM_18;
  settings.mTxPin = GPIO_NUM_19;

  // CAN 시작
  const uint32_t ret = ACAN_ESP32::can.begin(settings) ;
  
  if (ret == 0) {    
    // 성공했으면 관련 정보 출력
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
    // 실패시 에러 코드 출력
    Serial.print("Configuration error 0x") ;
    Serial.println(ret, HEX) ;    
  }
}

// CAN 데이터 전송 시간용
uint32_t sendTimeMs = 0;
// CAN 데이터에 카운터 증가용
uint8_t counter = 0;

void loop() {
  // CAN 메시지 프레임 변수
  CANMessage frame;
  
  // 1초 마다 CAN 데이터 전송
  if(millis() - sendTimeMs > 1000) {
    sendTimeMs = millis();

    frame.id = 0x100; // ID 0x100
    frame.rtr = 0; // RTR flag X
    frame.len = 8; // 8바이트 전송

    // 전송 성공때마다 1씩 증가해서 전송
    frame.data[0] = counter;
    frame.data[1] = 0x11;
    frame.data[2] = 0x22;
    frame.data[3] = 0x33;
    frame.data[4] = 0x44;
    frame.data[5] = 0x55;
    frame.data[6] = 0x66;
    frame.data[7] = 0x77;

    // 전송
    if (ACAN_ESP32::can.tryToSend(frame)) {
      counter++;
    }
    Serial.println("send...");
  }
  
  // 받은 데이터가 있다면
  if(ACAN_ESP32::can.receive(frame)) {
    Serial.print("**** Received ");

    // ID가 일반(Std)인지 확장(ext)인지 표시
    Serial.print(frame.ext ? "extended " : "standard ");

    // 데이터가 요청(remote)인지 일반 데이터인지 표시
    Serial.print(frame.rtr ? "remote " : "data ");

    // ID 표시
    Serial.print("frame, id 0x ");
    Serial.print(frame.id, HEX);

    // 데이터 길이 표시
    Serial.print("len 0x ");
    Serial.println(frame.len);
    
    // 데이터가 있다면 출력
    if(frame.len > 0) {
      Serial.println("  data: ");

      // 데이터 길이 만큼
      for(uint8_t i=0;i<frame.len;i++) {
        Serial.print(frame.data[i], HEX);
        Serial.print(" ");
      }
      Serial.println("");
    }
  }
  //delay(100);
}
//[출처] 아두이노 ESP32 - CAN 통신하기|작성자 천동이