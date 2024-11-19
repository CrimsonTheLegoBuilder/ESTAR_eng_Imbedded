#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Arduino.h"
#include "__estar_config.h"

struct Button {
  int sig, led;
  unsigned long last_sig;
  int state;  
  Button(int sig_ = 0, int led_ = 0) {
    pinMode(sig = sig_, INPUT_PULLUP);
    pinMode(led = led_, OUTPUT);
    state = 0;
  }
  int read();
  void blink(int x);
  int check(int& ent_state, int n);
  int run(int& ent_state, int x, int wait = 1000);
};

#endif