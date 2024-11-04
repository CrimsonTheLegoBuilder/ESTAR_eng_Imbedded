#ifndef __7SEG_H__
#define __7SEG_H__

#include "__estar_config.h"

struct SevenSeg {
  int n[8];
  SevenSeg(int a_ = 0, int b_ = 0, int c_ = 0, int d_ = 0, int e_ = 0, int f_ = 0, int g_ = 0, int h_ = 0) {
    n[0] = a_;
    n[1] = b_;
    n[2] = c_;
    n[3] = d_;
    n[4] = e_;
    n[5] = f_;
    n[6] = g_;
    n[7] = h_;
    pinMode(n[0], OUTPUT);
    pinMode(n[1], OUTPUT);
    pinMode(n[2], OUTPUT);
    pinMode(n[3], OUTPUT);
    pinMode(n[4], OUTPUT);
    pinMode(n[5], OUTPUT);
    pinMode(n[6], OUTPUT);
    pinMode(n[7], OUTPUT);
  }
  int print(int x, int p = 0);
};

struct TwoSeg {
  SevenSeg A, B;
  TwoSeg() {
    A = SevenSeg(
      A_0, 
      A_1, 
      A_2, 
      A_3, 
      A_4, 
      A_5, 
      A_6, 
      A_7
    );
    B = SevenSeg(
      B_0, 
      B_1, 
      B_2, 
      B_3, 
      B_4, 
      B_5, 
      B_6, 
      B_7
    );
  }
  int print(int xy);
};

#endif