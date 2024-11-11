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
    last_sig = 0;
  }
  int read();
  int check(int ent_state);
  void blink(int x);
};

#endif