#include "Arduino.h"
#ifndef __BUTTON_SOURCE__
#define __BUTTON_SOURCE__

#include "_button.h"

int Button::read() {
  int n = digitalRead(sig);
  if (n) digitalWrite(led, 0);
  else digitalWrite(led, 1);
  return n;
}
int Button::check(int n) {
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
void Button::blink(int x) {
  digitalWrite(led, x);
}

#endif