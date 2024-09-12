#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // 소프트웨어 시리얼 용 핀 지정
DFRobotDFPlayerMini myDFPlayer;              // 객체 생성
void printDetail(uint8_t type, int value) {
  Serial.println(type);
  Serial.println(value);
}

void setup()
{
  mySoftwareSerial.begin(9600);          // 소프트웨어 시리얼 통신 개시
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial), false) {  //객체 초기화
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //시리얼 통신용 타임아웃 시간 지정
  
  //----볼륨조절----      
  myDFPlayer.volume(5);  //0-30사이의 값을 인수로 입력.
  //myDFPlayer.volumeUp(); //볼륨을 1단계씩 키울 때 사용
  //myDFPlayer.volumeDown(); //볼륨을 1단계씩 내릴 때 사용
  
  //----이퀄라이즈 모드 지정시 사용 ----   
  //myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);    // 일반모드
  //myDFPlayer.EQ(DFPLAYER_EQ_POP);     // 팝모드
  //myDFPlayer.EQ(DFPLAYER_EQ_ROCK);    // 락모드
  //myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);    // 재즈모드
  //myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);  //클래식 모드
  myDFPlayer.EQ(DFPLAYER_EQ_BASS);     // BASS모드

  myDFPlayer.playLargeFolder(1, 1);
  
  //---디바이스 모드 지정시 사용: 위 모듈 사용시 별도로 지정하지 않아도 SD카드 모드로 지정되기 때문에 사용하지 않아도 됨 ----
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);   
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);       
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);      
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----mp3 모듈 제어 모드----
  //myDFPlayer.sleep();     //슬립모드
  //myDFPlayer.reset();     //슬립모드로부터 복귀
  //myDFPlayer.enableDAC();  //음악파일에서 디코딩 상태로 설정
  //myDFPlayer.disableDAC();  //음악파일을 디코딩 하지 않음
  //myDFPlayer.outputSetting(true, 15); //이 모듈에서는 적용되지 않음
  
  //----Mp3 재생----
  //myDFPlayer.next();  //다음 곡 재생
  //delay(1000);
  //myDFPlayer.previous();  //이전 곡 재생
  //delay(1000);
  //myDFPlayer.play(1);  //1번째 노래 재생(SD카드에 파일이 저장된 순서대로 재생됨)
  //delay(1000);
  //myDFPlayer.loop(1);  //첫번째 노래 반복재생
  //delay(1000);
  //myDFPlayer.pause();  //일시 정지
  //delay(1000);
  //myDFPlayer.start();  //다시 재생
  //delay(1000);
  //myDFPlayer.playFolder(15, 4);  //지정한 폴더의 트랙 재(15번 폴더의 4번 트랙재생)
  //delay(1000);
  //myDFPlayer.enableLoopAll(); //전체 MP3파일 재생(SD카드에 파일이 저장된 순서대로 쭉 재생)
  // delay(1000);
  // myDFPlayer.disableLoopAll(); //전체 MP3파일 재생 중지(enableLoopAll 함수 이후 실행해야 효과 적용 됨)
  // delay(1000);
  // myDFPlayer.playMp3Folder(4); //mp3폴더에 저장된 트랙을 재생(SD:/MP3/0004.mp3 재생) 트랙번호는 (0~65535) 지정 가능
  // delay(1000);
  // myDFPlayer.advertise(3); // 광고모드를 실행해서 SD:/ADVERT/0003.mp3 파일을 실행,  밑에 추가 설명 참고
  // delay(1000);
  // myDFPlayer.stopAdvertise(); //광고 모드 해제
  // delay(1000);
  // myDFPlayer.playLargeFolder(2, 999); //폴더내에 mp3파일이 여러개 여서 0001.mp3형식으로 저장했을 때 사용 
  // delay(1000);
  // myDFPlayer.loopFolder(5); //지정한 폴더엥 있는 모든 음악 순회재생(SD:/05번 폴더 순회 재생)
  // delay(1000);
  // myDFPlayer.randomAll(); //모든 음악파일 랜덤 재생
  // delay(1000);
  // myDFPlayer.enableLoop(); //현재 재생 중인 트랙을 반복 재생 모드로 지정
  // delay(1000);
  // myDFPlayer.disableLoop(); //현재 반복 재생 트랙을 반복재생 모드에서 해제
  // delay(1000);

  //----Read imformation----
  // Serial.println(myDFPlayer.readState()); //mp3 모듈 상태 읽어오기
  // Serial.println(myDFPlayer.readVolume()); //현재 볼륨값 읽어오기
  // Serial.println(myDFPlayer.readEQ()); //EQ세팅 읽어오기
  // Serial.println(myDFPlayer.readFileCounts()); //SD카드내의 전체 음악파일 개수 얻어오기
  // Serial.println(myDFPlayer.readCurrentFileNumber()); //현재 재생중인 폴더의 파일번호 얻어오기
  // Serial.println(myDFPlayer.readFileCountsInFolder(3)); //특정 폴더내의 파일 개수 얻어오기
}

void music_play(int fold, int num) {
  myDFPlayer.playLargeFolder(fold, num);
  delay(1000000);
}
int c = 0;
void loop()
{
  // static unsigned long timer = millis();
  
  // if (millis() - timer > 3000) {
  //   timer = millis();
  //   myDFPlayer.next();  //다음 곡 재생
  // }
  if (Serial.available()) {
    int f = Serial.parseInt();
    if (f / 100 == 8) {
      f -= 800;
      if (!f) myDFPlayer.volumeUp(); //볼륨을 1단계씩 키울 때 사용
      else myDFPlayer.volumeDown(); //볼륨을 1단계씩 내릴 때 사용
    }
    if (f / 100 == 9) {
      f -= 900;
      int fold = f / 10;
      int num = f % 10;
      myDFPlayer.playLargeFolder(fold, num);
    }
  }
  c++ ;
  // // if (myDFPlayer.available()) {
  // //   printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  // // }
  // delay(100);
}

