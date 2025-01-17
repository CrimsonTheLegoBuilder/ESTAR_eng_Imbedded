//송신기

//라이브러리
#include <ACAN_ESP32.h> 

//1초라는 타이밍을 생성하기 위한 시간을 담는 변수
unsigned long t = 0;
uint32_t id = 0x404;

void setup() {
  Serial.begin(115200); //나 컴퓨터와 USB로 통신할거야!
  //캔통신의 통신속도를 결정하는 부분
  //125k(125000UL), 250k(250000UL), 500k(500000UL)
  ACAN_ESP32_Settings settings(500000UL); 

  //실제로 통신할거기 때문에 루프백모드를 설정하면 안된다
  //settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode;
  
  //Default Tx:GPIO4, Rx:GPIO5
  settings.mRxPin = GPIO_NUM_19; //TX2
  settings.mTxPin = GPIO_NUM_18; //RX2

  //통신속도와 연결핀을 기준으로 캔통신 초기화
  const uint32_t ret = ACAN_ESP32::can.begin(settings) ;

  //ESP32에 설정된 캔통신 파라미터가 ~~~게 되었다라는것을 단순 프린트해준다!
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
  id = 0x404;
}

void loop() {
  //이거는 하나 필요함!
  CANMessage frame;
  if (Serial.available()) {
    int x = Serial.parseInt();
    while (Serial.available() > 0) {
      Serial.read();
    }
    Serial.println(x);
    if (x == 0) id = 0x400;
    if (x == 1) id = 0x401;
    if (x == 2) id = 0x402;
    if (x == 3) id = 0x403;
    if (x == 4) id = 0x404;
    Serial.println(id);
  }

  if(millis() - t > 1000){
    t = millis();
    //1000ms마다 이부분이 반복실행된다!

    //호출하고자하는 슬레이브의 ID
    //standard일때(11bit)
    //0x000(0) ~ 7FF(2,047)
    //extended일때(29bit)
    //0x000(0) ~ 0x1FFFFFFF(536,870,911)
    frame.id = id;
    frame.rtr = 0; //확실히 이게 무슨 기능을 하는지는 모르겠음!
    frame.len = 8; //내가 보낼 데이터의 길이

    frame.data[0] = 'N';
    frame.data[1] = 'O';
    frame.data[2] = 'C';
    frame.data[3] = 'K';
    frame.data[4] = 'A';
    frame.data[5] = 'N';
    frame.data[6] = 'D';
    frame.data[7] = 'A';

    //전송
    if (ACAN_ESP32::can.tryToSend(frame)) {
      //성공 
      Serial.println("전송에 성공했습니다");
    }   
  }
    if(ACAN_ESP32::can.receive(frame)) {
    Serial.println("RCV");
    //frame.ext : ID종류가 29bit인가? 아니면 11bit인가
    //frame.rtr : 리모트인가? 아니면 데이터인가
    //frame.id : 아이디
    //frame.len : 수신데이터 길이
    //frame.data : 수신데이터(배열)
    if(frame.ext){
      Serial.println("EXTENDED ID");
    }else{
      Serial.println("STANDARD ID");
    }

    if(frame.rtr){
      Serial.println("RTR : REMOTE!");
    }else{
      Serial.println("RTR : DATA!");
    }

    Serial.print("received ID : ");
    Serial.println(frame.id,HEX);

    Serial.print("data len : ");
    Serial.println(frame.len);

    //결과를 16진수로출력
    Serial.print("HEX DATA : ");
    for(int i = 0;i<frame.len;i++){
      Serial.print(frame.data[i],HEX);
      Serial.print(", ");
    }
    Serial.println();

    //결과를 아스키코드로출력
    Serial.print("    DATA : ");
    for(int i = 0;i<frame.len;i++){
      Serial.print(frame.data[i]);
      Serial.print(", ");
    }
    Serial.println();
  } 
  
}