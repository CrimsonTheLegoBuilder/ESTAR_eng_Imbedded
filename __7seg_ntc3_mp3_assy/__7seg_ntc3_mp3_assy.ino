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
DFRobotDFPlayerMini myDFPlayer;
unsigned long tmp_period;
unsigned long btn_period;

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(13, OUTPUT);
  if (!myDFPlayer.begin(Serial3), false) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(15);
  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  Serial.println(F("init done."));
  //myDFPlayer.playLargeFolder(1, 1);

  tmp_period = 0;
  btn_period = 0;
}

void music_play(int fold, int num) {
  // myDFPlayer.playFolder(fold, num);
  myDFPlayer.playLargeFolder(fold, num);
  //delay(1000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = T.read_temper(1);
  Serial.println(t);
  if (millis() - tmp_period >= 5000) {
    S.print(t);
    tmp_period = millis();
  }
  int n;
  n = b1.read();
  int c1 = b1.check(n);
  n = b2.read();
  int c2 = b2.check(n);
  n = b3.read();
  int c3 = b3.check(n);
  n = b4.read();
  int c4 = b4.check(n);
  n = b5.read();
  int c5 = b5.check(n);
  n = b6.read();
  int c6 = b6.check(n);
  Serial.println(c1);
  Serial.println(c2);
  Serial.println(c3);
  Serial.println(c4);
  Serial.println(c5);
  Serial.println(c6);
  if (c1 == -1 || c2 == -1 || c3 == -1 || c4 == -1 || c5 == -1 || c6 == -1) {
    digitalWrite(13, HIGH);
    //music_play(1, 3);
    delay(10);
    digitalWrite(13, LOW);
  }
  if (c1 == 2 || c2 == 2 || c3 == 2 || c4 == 2 || c5 == 2 || c6 == 2) {
    if (!change) {
      state = (state + 1) % 2;
      //music_play(1, 4);
      delay(10);
      change = 1;
    }
  }
  else change = 0;
  if (state == 1) {
    cnt = (cnt + 1) % 39;
    if (b1.read()) b1.blink(cnt / 20);
    if (b2.read()) b2.blink(cnt / 20);
    if (b3.read()) b3.blink(cnt / 20);
    if (b4.read()) b4.blink(cnt / 20);
    if (b5.read()) b5.blink(cnt / 20);
    if (b6.read()) b6.blink(cnt / 20);
    Serial.println(cnt);
    Serial.println(cnt / 20);
  }
  delay(50);
}
