#ifndef _TURNTABLE_H_
#define _TURNTABLE_H_

#include <Arduino.h>

/*
모터의 속도는 61rpm
버튼이 물리적으로 눌렸을 때 1.1mm 정도를 누른 후 1.8mm를 진행하기 전에 작동을 멈춰야 함.

*/

namespace table {
  int left_most();
  int right_most();
  void turn(const int& dir);
  void dir_init(int& cnt);
  int table_main();
}

#endif