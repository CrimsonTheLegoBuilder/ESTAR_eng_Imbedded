// #ifndef _MOTOR_SOURCE_
// #define _MOTOR_SOURCE_

// #include "motorControl.h"

// Motor::init() {
//   dir_pin = MOTOR2_DIR;
//   spd_pin = MOTOR2_SPD;
//   pinMode(dir_pin, OUTPUT); 
//   pinMode(spd_pin, OUTPUT);
//   return;
// }

// Motor::write(int dir, int spd) {
//   /*
//   각도를 출력으로 변환하는 함수, 이 부분이 캘리브레이션이 필요함.
//   */
//   digitalWrite(dir_pin, dir); 
//   analogWrite(spd_pin, spd);
//   return;
// }

// #endif