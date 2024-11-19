#include "__estar_config.h"
#include "_7seg.h"
#include "_thermo.h"
#include "_button.h"
#include "DFRobotDFPlayerMini.h"

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
int change = 0;
//DFRobotDFPlayerMini myDFPlayer;
unsigned long tmp_period;
unsigned long btn_period;

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
  //Serial.println(F("init done."));
  //myDFPlayer.playLargeFolder(1, 1);

  tmp_period = 0;
  btn_period = 0;
  state = 1;
  pinMode(L7, OUTPUT);
  pinMode(L8, OUTPUT);
}

// void music_play(int fold, int num) {
//   // myDFPlayer.playFolder(fold, num);
//   myDFPlayer.playLargeFolder(fold, num);
//   //delay(1000000);
// }

void loop() {
  // put your main code here, to run repeatedly:
  float t = T.read_temper(1);
  Serial.println(t);
  Serial.println(state == STANDBY_MODE ? "STANDBY" : state == SETTING_MODE ? "SETTING" : "DELIVER");
  if (millis() - tmp_period >= 5000) {
    S.print(t);
    tmp_period = millis();
  }
  int c1 = b1.run(state, cnt / 20);
  int c2 = b2.run(state, cnt / 20);
  int c3 = b3.run(state, cnt / 20);
  int c4 = b4.run(state, cnt / 20);
  int c5 = b5.run(state, cnt / 20);
  int c6 = b6.run(state, cnt / 20);
  Serial.print(c1);
  Serial.print(" ");
  Serial.print(c2);
  Serial.print(" ");
  Serial.print(c3);
  Serial.print(" ");
  Serial.print(c4);
  Serial.print(" ");
  Serial.print(c5);
  Serial.print(" ");
  Serial.print(c6);
  Serial.println(" ");
  cnt = (cnt + 1) % 40;
  if (c1 == 3 || c2 == 3 || c3 == 3 | c4 == 3 || c5 == 3 || c6 == 3) {
    b1.state = 0;
    b2.state = 0;
    b3.state = 0;
    b4.state = 0;
    b5.state = 0;
    b6.state = 0;
    if (state == SETTING_MODE) state = STANDBY_MODE;
    else if (state == STANDBY_MODE) state = SETTING_MODE;
    else if (state == DELIVER_MODE) {
      digitalWrite(L7, 0);
      digitalWrite(L8, 0);
      state = SETTING_MODE;
    }
  }
  if (c1 == 2 || c2 == 2 || c3 == 2 | c4 == 2 || c5 == 2 || c6 == 2) {
    state = DELIVER_MODE;
  }
  delay(500);
}
