#include "__estar_config.h"
#include "_7seg.h"
#include "_thermo.h"
#include "_button.h"
// #include "DFRobotDFPlayerMini.h"

TwoSeg S;
Thermo T(B_pin, R_pin, W_pin);
Button b1 = Button(B1, L1);
Button b2 = Button(B2, L2);
Button b3 = Button(B3, L3);
Button b4 = Button(B4, L4);
Button b5 = Button(B5, L5);
Button b6 = Button(B6, L6);
int cnt = 0;
int prev_state = 0;
int state = 0;
int mode = STANDBY_MODE;
int change = 0;
// DFRobotDFPlayerMini myDFPlayer;
unsigned long tmp_time;
unsigned long btn_time;
const unsigned long wait_time = 3000;
const int btnPin[6] = { B1, B2, B3, B4, B5, B6 };
const int lightPin[6] = { L1, L2, L3, L4, L5, L6 };

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(13, OUTPUT);
  // if (!myDFPlayer.begin(Serial3), false) {
  //   Serial.println(F("Unable to begin:"));
  //   Serial.println(F("1.Please recheck the connection!"));
  //   Serial.println(F("2.Please insert the SD card!"));
  //   while(true);
  // }
  // Serial.println(F("DFPlayer Mini online."));
  // myDFPlayer.setTimeOut(500);
  // myDFPlayer.volume(15);
  // myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  // Serial.println(F("init done."));
  //myDFPlayer.playLargeFolder(1, 1);

  tmp_time = 0;
  btn_time = 0;
  state = 1;
  pinMode(L7, OUTPUT);
  pinMode(L8, OUTPUT);
}

// void music_play(int fold, int num) {
//   // myDFPlayer.playFolder(fold, num);
//   myDFPlayer.playLargeFolder(fold, num);
//   //delay(1000000);
// }

int read_all_button() {
  for (int i = 0; i < 6; i++) {
    if (!digitalRead(btnPin[i])) return 1;
  }
  return 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - tmp_time >= 500) {
  float t = T.read_temper(1);
  S.print(t);
  Serial.println(t);
  Serial.println(state == STANDBY_MODE ? "STANDBY" : state == SETTING_MODE ? "SETTING" : "DELIVER");
    tmp_time = millis();
  }
  if (mode == SETTING_MODE) {
    b1.blink(btn_time / 25);
    b2.blink(btn_time / 25);
    b3.blink(btn_time / 25);
    b4.blink(btn_time / 25);
    b5.blink(btn_time / 25);
    b6.blink(btn_time / 25);
    btn_time = (btn_time + 1) % 50;
  }
  for (int i = 0; i < 6; i++) {
    int x = digitalRead(btnPin[i]);
    Serial.println(btnPin[i]);
    if (i < 5) Serial.print(x), Serial.print(" ");
    else Serial.print(x), Serial.println(" ");
    if (!x) {
      digitalWrite(lightPin[i], 1);
      unsigned long s = millis();
      while (1) {
        // if (mode == SETTING_MODE) {
        //   b1.blink(btn_time / 25);
        //   b2.blink(btn_time / 25);
        //   b3.blink(btn_time / 25);
        //   b4.blink(btn_time / 25);
        //   b5.blink(btn_time / 25);
        //   b6.blink(btn_time / 25);
        //   btn_time = (btn_time + 1) % 50;
        // }
        x = digitalRead(btnPin[i]);
        if (x) {
          unsigned long cur = millis();
          unsigned long diff = cur - s;
          if (diff < wait_time) {//물 토출 혹은 설정 변경
            if (mode == STANDBY_MODE) {
              mode = DELIVER_MODE;
              Serial.println("deliver water");
              digitalWrite(lightPin[i], 1);
              digitalWrite(L7, 1);
              digitalWrite(L8, 1);
              // music_play(1, 3);
              s = millis();
              while (1) {
                cur = millis();
                diff = cur - s;
                if (read_all_button()) {
                  digitalWrite(lightPin[i], 0);
                  digitalWrite(L7, 0);
                  digitalWrite(L8, 0);
                  mode = STANDBY_MODE;
                  return;
                }
                if (diff > SZ_SHORT) {
                  digitalWrite(lightPin[i], 0);
                  digitalWrite(L7, 0);
                  digitalWrite(L8, 0);
                  mode = STANDBY_MODE;
                  delay(50);
                  return;
                }
                //물 토출 시작
                //신호음 울리기
                //대기 시간까지 다시 대기, 취소 과정 집어넣기
              }
            }
            else if (mode == SETTING_MODE) {
              //뭔가 설정을 바꾸기
              digitalWrite(lightPin[i], 1);
              // music_play(1, 3);
              delay(20);
              digitalWrite(lightPin[i], 0);
            }
          }
          else {//설정 모드로 진입 혹은 탈출
            //모드를 설정으로 바꾸고 리턴
            if (mode == SETTING_MODE) mode = STANDBY_MODE;
            else if (mode == STANDBY_MODE) mode = SETTING_MODE;
            Serial.println("mode changed");
            // music_play(1, 4);
            if (mode == STANDBY_MODE) {
              b1.blink(0);
              b2.blink(0);
              b3.blink(0);
              b4.blink(0);
              b5.blink(0);
              b6.blink(0);
            }
            return;
          }
          break;
        }
        unsigned long cur = millis();
        unsigned long diff = cur - s;
        if (diff > wait_time) {
          if (mode == SETTING_MODE) mode = STANDBY_MODE;
          else if (mode == STANDBY_MODE) mode = SETTING_MODE;
          Serial.println("mode changed");
          // music_play(1, 4);
          while (1) {
            x = digitalRead(btnPin[i]);
            if (x) break;
            btn_time = (btn_time + 1) % 20;
            b1.blink(btn_time / 10);
            b2.blink(btn_time / 10);
            b3.blink(btn_time / 10);
            b4.blink(btn_time / 10);
            b5.blink(btn_time / 10);
            b6.blink(btn_time / 10);
            
            Serial.println("DEBUG::");
          }
          if (mode == STANDBY_MODE) {
            b1.blink(0);
            b2.blink(0);
            b3.blink(0);
            b4.blink(0);
            b5.blink(0);
            b6.blink(0);
          }
          return;
        }

      }
      break;//동시 버튼 입력은 허용하지 않음
    }
  }
  
  //delay(500);
}
