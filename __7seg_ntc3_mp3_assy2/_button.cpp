#include "Arduino.h"
#ifndef __BUTTON_SOURCE__
#define __BUTTON_SOURCE__

#include "_button.h"

int Button::read() {
  int n = digitalRead(sig);
  //if (n) digitalWrite(led, 0);
  //else digitalWrite(led, 1);
  return n;
}
void Button::blink(int x) {
  digitalWrite(led, x);
}
int Button::check(int& ent_state, int n) {
  if (n) {//눌리지 않았다면?
    if (ent_state != SETTING_MODE && state == 3) {
      ent_state = SETTING_MODE;
      state = 1;
      return 0;
    }
    if (ent_state == SETTING_MODE) {
      return 0;
    }
    if (state == 1) {
      state = 2;
      last_sig = millis();
      return 2;
    }
    return 0;
  }
  if (state == 0) {//눌렀는데 최초라면?
    state = 1;
    last_sig = millis();
    return 1;
  }
  if (state == 2) {
    last_sig = millis();
    return -1;
  }
  unsigned long cur = millis();
  if (cur - last_sig >= 3000) {
    state = 3;
    last_sig = millis();
    return 3;
  }
  else return 1;
}
int Button::run(int& ent_state, int x, int wait) {
  int r = read();
  int n = check(ent_state, r);
  Serial.print("s: ");
  Serial.print(state);
  Serial.print(" ");
  // if (n == 3) {
  //   ent_state == SETTING_MODE;
    
  // }
  if (state == 2) {
    if (n == 1) {
      digitalWrite(L7, 0);
      digitalWrite(L8, 0);
      blink(0);
      state = 0;
      return 0;
    }
    unsigned long cur = millis();
    if (cur - last_sig >= wait) {
      ent_state = STANDBY_MODE;
      state = 0;
      digitalWrite(L7, 0);
      digitalWrite(L8, 0);
    }
    else {
      digitalWrite(L7, 1);
      digitalWrite(L8, 1);
    }
  }
  if (state == 2 || !r || n == 1 || n == 2) blink(1);
  else if (n == 3 || ent_state == SETTING_MODE) blink(x);
  else blink(0);
  return n;
}

#endif