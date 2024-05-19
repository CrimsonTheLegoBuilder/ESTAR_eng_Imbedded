#include "Arduino.h"
#ifndef _SSR_SOURCE_
#define _SSR_SOURCE_

int htpmp = 4, ptc = 5, fan = 6;

#include "ssrControl.h"

int ssr::init(int heatpump_, int ptc_, int fan_) {
  htpmp = heatpump_;
  ptc = ptc_;
  fan = fan_;
  pinMode(htpmp, OUTPUT);
  pinMode(ptc, OUTPUT);
  pinMode(fan, OUTPUT);
  return 0;
}

bool ssr::detect_disturbance() {
  if (digitalRead(detector) == LOW) return 1;
  else return 0;
  //이전 상태와 읽어낸 값이 다르다면 그 때 변화했음을 반환?
}

int ssr::detect_door(int& prev) {
  if (digitalRead(detector) != prev) {
    if (prev == HIGH) {//door open
      prev = LOW;
      return 1;
    }
    else {//door close
      prev = HIGH;
      return 0;
    }
  }
  return -1;//no change
}

int ssr::ptc_control(int w) {//PID control
  /*
  
  transfer fn'
  
  */
  int x = map(w, 0, 100, 0, 255);
  analogWrite(ptc, x);
  return 0;
}

int ssr::heatpump_control(int w) {//ON/OFF control
  /*
  
  transfer fn'
  
  */
  int x = (w > 50) * 5;
  digitalWrite(htpmp, x);
  return 0;
}

int ssr::fan_control(int w) {
  /*
  
  transfer fn'
  
  */
  int x = map(w, 0, 100, 0, 255);
  analogWrite(ptc, x);
  return 0;
}

int ssr::turn_on(int w) {
  ssr::heatpump_control(w);
  ssr::ptc_control(w);
  ssr::fan_control(w);
  return 1;
}
int ssr::turn_off() {
  ssr::heatpump_control(0);
  ssr::ptc_control(0);
  ssr::fan_control(0);
  return 0;
}
int ssr::slowly_down(int& w) {
  if (w <= 0) return 0;
  if (w > 0) brt = max(0, w - 10);//서서히 내린다
  ssr::heatpump_control(w);
  ssr::ptc_control(w);
  ssr::fan_control(w);
  return -1;//if return -1, slowly power down
}

int ssr::run_dryer(const int& mode, const int& door_state, const int& door, int& w) {//
/* 이 함수는 반복 수행 되는 루프 안에 있게 된다. 계층 구조를 나눴을 때 
 * 하드웨어와 근접한 함수는 내부에서만 다루고 ino 파일에서 히터를 작동할 때는
 * 보다 소프트웨어 적으로 접근할 수 있도록 함수를 디자인 한다.
 * mode : 계속 작동을 할 것인지, 서서히 멈췄다가 다시 동작을 시킬 것인지, 완전히 끌 것인지
 * door_state : 문이 열려있는가
 * w : 출력을 지정하는 변수
 * 
 *
 *
 *
 */
  if (!door_state) return -2;
  switch (mode) {
    case NORMAL: return ssr::turn_on(w);
    case DECREASE: return ssr::slowly_down(w);
    case STOP: return ssr::turn_off();
    default:
    Serial.println("error occured");
    return ssr::turn_off(brt);
  }
  return 0;
}

#endif