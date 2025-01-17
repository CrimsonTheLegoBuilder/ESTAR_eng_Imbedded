#ifndef __7SEG_SOURCE__
#define __7SEG_SOURCE__

#include "_7seg.h"

int SevenSeg::print(int x, int p) {
  if (x > 9) return 0;
  if (x == -1) {
    digitalWrite(n[0], 0);
    digitalWrite(n[1], 0);
    digitalWrite(n[2], 0);
    digitalWrite(n[3], 0);
    digitalWrite(n[4], 0);
    digitalWrite(n[5], 0);
    digitalWrite(n[6], 0);
    digitalWrite(n[7], 0);
    return 0;
  }
  if (p) digitalWrite(n[4], 1); 
  else digitalWrite(n[4], 0);
  if (x == 0) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 0);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 1);
  }
  if (x == 1) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 0);
    digitalWrite(n[2], 0);
    digitalWrite(n[3], 0);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 0);
    digitalWrite(n[7], 0);
  }
  if (x == 2) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 0);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 0);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 1);
  }
  if (x == 3) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 0);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 0);
  }
  if (x == 4) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 0);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 0);
    digitalWrite(n[7], 0);
  }
  if (x == 5) {
    digitalWrite(n[0], 0);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 0);
  }
  if (x == 6) {
    digitalWrite(n[0], 0);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 1);
  }
  if (x == 7) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 0);
    digitalWrite(n[3], 0);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 0);
    digitalWrite(n[7], 0);
  }
  if (x == 8) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 1);
  }
  if (x == 9) {
    digitalWrite(n[0], 1);
    digitalWrite(n[1], 1);
    digitalWrite(n[2], 1);
    digitalWrite(n[3], 1);
    digitalWrite(n[5], 1);
    digitalWrite(n[6], 1);
    digitalWrite(n[7], 0);
  }
  return 1;
}

int TwoSeg::print(int xy) {
  if (xy < 0 || xy > 99) {
    A.print(8, 1);
    B.print(8, 1);
    return 0;
  }
  int x = xy / 10;
  int y = xy % 10;
  if (x > 0) A.print(x);
  else A.print(-1);
  B.print(y);
  return 1;
}

#endif