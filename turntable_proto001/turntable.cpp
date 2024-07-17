#ifndef _TURNTABLE_SOURCE_
#define _TURNTABLE_SOURCE_

#include "turntable.h"

/*
모터의 속도는 61rpm
버튼이 물리적으로 눌렸을 때 1.1mm 정도를 누른 후 1.8mm를 진행하기 전에 작동을 멈춰야 함.

갑작스럽게 방향을 바꾸는 동작을 최대한 피해야함. 멈춰야하는 지점을 예측해야한다는 의미.
*/

int table::left_most() {
  //return left sensor output == 1
}
int table::right_most() {
  //return right sensor output == 1
}
void table::turn(const int& dir) {
  //out motor controller (dir) :: 1 : ccw, -1 : cw, 0 : stop
}

void table::dir_init(int& dir, int& cnt) {
  cnt = 0;
  dir = 1;//ccw
  while (1) {
    if (right_most()) {
      turn(0);
      return;
    }
    turn(-1);
  }
}

int table::table_main(int& ang, float& r) {
  int unit = 10;//슬릿 1개당 각도 
  int cnt;//슬릿 수
  int dir;//회전 방향
  dir_init(dir, cnt);
  while (1) {
    /*
      대기 시간 만큼 회전부가 진행함. R * PI * vt
      vt는 ms 단위? 3.6rpm / 60 / 1000
    */
    int l_ = left_most();
    int r_ = right_most();
    
    if (l_ || r_) {
      turn(0);//stop
      dir *= -1;
    }
    else if (ang <= cnt * unit) {
      turn(0);//stop
      dir *= -1;
    }



    float v = 3.6 * r / 60 / 1000;
    delay(1. / v);
  }
}

#endif