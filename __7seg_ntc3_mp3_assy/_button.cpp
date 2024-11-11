#include "Arduino.h"
#ifndef __BUTTON_SOURCE__
#define __BUTTON_SOURCE__

#include "_button.h"

int Button::read() {
  int n = digitalRead(sig);
  // if (n) digitalWrite(led, 0);
  // else digitalWrite(led, 1);
  return n;
}
void Button::blink(int x) {
  digitalWrite(led, x);
}
int Button::check(int ent_state, int x) {
  int n = read();
  switch (ent_state) {
    case NORMAL_MODE:
      if (n) {
        digitalWrite(led, 0);
        if (!state) {
          state = 0;
          return -1;
        }
        state = 0;
        return 0;
      }
      else {
        digitalWrite(led, 1);
        
      }
      break;
    case SETTING_MODE:
      break;
  }
  if (n) {
    last_sig = millis();
    if (state == 1) {
      state = 0;
      return -1;
    }
    state = 0;
    return 0;
  }
  if (!state) {
    state = 1;
    last_sig = millis();
    return 1;
  }
  unsigned long cur = millis();
  if (cur - last_sig >= 3000) {
    //last_sig = millis();
    state = 2;
    return 2;
  }
  else return 1;
}

/*

누른 후에 1초 정도 멈췄다가 신호가 바뀌어야 함. 폴링 함수 안에서는 오로지 버튼의 상태를 점검하는 변수만 있어야함.
설정 모드이고 눌려있지 않디면 깜빡임
설정 모드이지만 눌렸다면 누른 상태 유지
일반 모드라면 마지막으로 눌린 시간을 추적해서 1초가 지나기 전에 누르면 누름 취소, 아니면 누름으로 상태를 변경


*/

#endif