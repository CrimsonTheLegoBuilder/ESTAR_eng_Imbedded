#include "Arduino.h"
#ifndef _SSR_SOURCE_
#define _SSR_SOURCE_

int htpmp = 4, ptc = 5, fan = 6;

#include "ssrControl.h"

void ssr::init() {
  pinMode(SSR_1, OUTPUT);
  pinMode(SSR_2, OUTPUT);
  pinMode(SSR_3, OUTPUT);
}

void ssr::ir_control(int w) {//PID control
  /*
  
  transfer fn'
  
  */
  int x = map(w, 0, 100, 0, 255);
  analogWrite(SSR_2, x);
}

void ssr::heatpump_control(int w) {//ON/OFF control
  digitalWrite(SSR_1, w);
}

void ssr::fan_control(int w) {
  analogWrite(SSR_3, w);
}

void ssr::turn_on(int w) {
  ssr::heatpump_control(w);
  ssr::ir_control(w);
  ssr::fan_control(w);
}
void ssr::turn_off() {
  ssr::heatpump_control(0);
  ssr::ir_control(0);
  ssr::fan_control(0);
}

void ssr::run_dryer(bool run_state) {
  if (!run_state) {
    ssr::turn_off();
    return;
  }

}

/*
#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// 핀 설정
#define ONE_WIRE_BUS 2  // 온도 센서 데이터 핀
#define HEATER_PIN 3    // 히터 제어 핀

// 목표 온도 설정
double setpoint = 25.0; // 목표 온도 (섭씨)

// PID 변수
double input, output;
double Kp = 2.0, Ki = 5.0, Kd = 1.0;
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// 온도 센서 초기화
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // 히터 핀을 출력 모드로 설정
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);

  // 시리얼 모니터 시작
  Serial.begin(115200);

  // 온도 센서 시작
  sensors.begin();

  // PID 시작
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255); // 출력 범위 설정 (PWM 제어)

  // 초기 온도 읽기
  sensors.requestTemperatures();
  input = sensors.getTempCByIndex(0);
}

void loop() {
  // 온도 읽기
  sensors.requestTemperatures();
  input = sensors.getTempCByIndex(0);
  
  // PID 계산
  myPID.Compute();

  // 히터 제어
  analogWrite(HEATER_PIN, output);

  // 디버깅 출력
  Serial.print("Current Temperature: ");
  Serial.print(input);
  Serial.print("C, Heater Output: ");
  Serial.println(output);

  delay(1000); // 1초 대기
}

*/

#endif