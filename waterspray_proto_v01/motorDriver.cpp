/*
This file is control both motor for rotate turntable and photo sensor for count teeth of turntable.

단위 각도는 teeth 변수를 전달 받아서 계산하도록 만든다. 단위 각도는 왕복 회전 알고리즘에서 사용된다.
왕복해야 하는 각도의 범위가 주어지면, 해당 범위 안에서 속도를 조금씩 올리면서 움직이다가
중간 지점 혹은 도착 예정 지점에 도착하기 전에 속도가 0이 되도록 조정해서 멈출 수 있도록 한다.

회전 알고리즘: 속도를 올리는 부분은 간단하게 구현하면 됨.
속도를 줄일 때가 문제가 됨. 부드럽게 도착하도록 하는 방법은? 여기서 PID를 사용해야하나?

*/

#ifndef _MOTOR_SOURCE_
#define _MOTOR_SOURCE_

#include "motorDriver.h"

Motor* Motor::motor1 = nullptr;
Motor* Motor::motor2 = nullptr;

Motor motor1_ = Motor(MOTOR1_DIR, MOTOR1_SPD, BTN1_PIN, BTN2_PIN, PHOTO_PIN, 36);
Motor motor2_ = Motor(MOTOR2_DIR, MOTOR2_SPD, BTN3_PIN, BTN4_PIN);

#ifdef DEBUG
float SPD[36];
#endif

#define a1_ 52.492
#define b1_ 45.478
#define a2_ 0.0192
#define b2_ 0.8915

void Motor::begin() {
  if (motor1 == nullptr) {
    motor1 = this;
    attachInterrupt(digitalPinToInterrupt(sensor_pin), photoISR1, RISING);
    // attachInterrupt(digitalPinToInterrupt(btn_pin1), btnISR1, RISING);
    //attachInterrupt(digitalPinToInterrupt(btn_pin2), btnISR2, RISING);
  }
  else if (motor2 == nullptr) {
    motor2 = this;
    attachInterrupt(digitalPinToInterrupt(btn_pin1), sensorISR2, RISING);
  }
}

// void Motor::set_speed(int speed) { spd = speed; analogWrite(spd_pin, spd); }

void Motor::set_speed(int speed) { spd = speed; ledcWrite(spd_pin, speed); }

int Motor::get_pow(float speed) { return speed * a1_ + b1_; }

float Motor::get_speed(int pow) { return pow * a2_ - b2_; }

void Motor::set_speed(float rpm_) { spd = Motor::get_pow(rpm_); ledcWrite(spd_pin, spd); }

void Motor::set_direction(int direction) { dir = direction; digitalWrite(dir_pin, dir); }

void Motor::toggle() { dir ^= 1; digitalWrite(dir_pin, dir); }

void Motor::set_point(int point) { target = point; }



void Motor::handle_pulse() {
  unsigned long current_time = micros();
  if (current_time - last_pulse_time >= debounce_delay) {
    pulse_interval = current_time - last_pulse_time;
    last_pulse_time = current_time;
    cnt = (cnt + 1) % teeth;
  }
}

void Motor::stop() {
  set_speed(0);
}

float Motor::rad() {
  return 2 * PI * cnt / teeth;
}

float Motor::degree() {
  return 360 * cnt / teeth;
}

void Motor::count_() {
  // Serial.print("PULSE: ");
  // Serial.println(pulse_interval);
  if (pulse_interval > 0) {
    rpm = (1000000.0 / pulse_interval) * (60.0 / teeth);
    // Serial.print("RPM: ");
    // Serial.println(rpm);
    // Serial.print("pulse_interval: ");
    // Serial.println(pulse_interval);
    pulse_interval = 0; // reset pulse interval
    // Serial.print("cnt++:: ");
    // Serial.println(cnt);
  }
}

void IRAM_ATTR Motor::photoISR1() {//wtf?
  if (motor1 != nullptr) {
    motor1->handle_pulse();
  }
}

void IRAM_ATTR Motor::btnISR1() {//wtf?
  if (motor1 != nullptr) {
    unsigned long current_time = millis();
    if (current_time - motor1->last_interrupt_time > btn_debounce_delay) {
      motor1->stop();
      motor1->direction_changed = true;
      motor1->last_interrupt_time = current_time;
#ifdef DEBUG 
      motor1->interrupt_debugger++;
#endif
    }
  }
//   if (motor1 != nullptr) {
//     //portENTER_CRITICAL_ISR();
//     motor1->stop();
//     motor1->direction_changed = true;
// #ifdef DEBUG 
//     motor1->interrupt_debugger++;
// #endif
//     //portEXIT_CRITICAL_ISR();
//   }
}

void IRAM_ATTR Motor::btnISR2() {//wtf?
  if (motor1 != nullptr) {
    motor1->stop();
    Serial.println("FUCK::");
  }
}

void Motor::sensorISR2() {
  if (motor2 != nullptr) {
    motor2->toggle();
  }
}

// int Motor::mission(int degree) {

// }

void Motor::DEBUG_() {
  Serial.print("spd:: ");
  Serial.println(spd);
  Serial.print("dir:: ");
  Serial.println(dir);
  Serial.print("rpm:: ");
  Serial.println(rpm);
  Serial.print("cnt:: ");
  Serial.println(cnt);
  // Serial.print("photo:: ");
  // Serial.println(digitalRead(sensor_pin));
  Serial.print("btn1:: ");
  Serial.println(digitalRead(btn_pin1));
#ifdef DEBUG 
  Serial.print("interrupt debugger:: ");
  Serial.println(interrupt_debugger);
#endif
}

#endif

// void onPulse() {
//   unsigned long current_pulse_time = micros();
//   if (current_pulse_time - last_pulse_time >= debounce_delay) {
//     pulse_interval = (current_pulse_time - last_pulse_time);
//     last_pulse_time = current_pulse_time;
//     //if (!dir) return;
//     if (cnt >= teeth) {
//       direction_changed = true;
//     }
//   }
// }