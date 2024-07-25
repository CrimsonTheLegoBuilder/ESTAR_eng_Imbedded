#ifndef _WTS_CONTROL_SOURCE_
#define _WTS_CONTROL_SOURCE_

#include "Arduino.h"
#include "wtsControl.h"
const int VDD = 3300;

/*
구조체로 두 개 이상의 온습도 센서를 관리해야한다. 구조 변경 필요.
*/

int wts::init(Data& data) {
  pinMode(t_pin, INPUT);
  pinMode(h_pin, INPUT);
  int cnt = 100;
  while (cnt--) {
    if (wts::check(data)) { Serial.println("WTS3535 initialized."); return 1; }
    Serial.println("wait...");
  }
  Serial.println("something wrong.");
  return 0;
}

int wts::measure(Data& data) {
  int vt, vrh;
  vt = analogRead(t_pin);
  vrh = analogRead(h_pin);
  Serial.println(vt);
  Serial.println(vrh);
  vt = map(vt, 0, 4095, 0, VDD);
  vrh = map(vrh, 0, 4095, 0, VDD);
  Serial.println(vt);
  Serial.println(vrh);
  float t = -66.875 + 218.75 * vt / VDD;
  float h = -(19.7 / 0.54) + (100 / 0.54 * vrh / VDD);
  data.temp = t;
  data.humi = h;
  return 1;
}

int wts::check(Data& data) {
  bool f = 1;
  wts::measure(data);
  /*
  if cannot read anything: return false;
  */
  Serial.println(f);
  return f;
}



#endif