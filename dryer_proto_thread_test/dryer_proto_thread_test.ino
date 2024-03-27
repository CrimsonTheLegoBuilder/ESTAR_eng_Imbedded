#include "_config.h"
#include <DHT.h>
#include <SoftwareSerial.h>
#include <FastLED.h>
#include "ledControl.h"
#include "pidControl.h"

// RGB_PIN 2
// DHT_PIN 3

int ledPin = 13;
extern int color;

// pid parameters setting
PID temper_pid = PID(.2, .1, .05, 1e-4, .0);
// PID(Kp, Ki, Kd, dt, low_limit, high_limit);
// high_limit's default value = 70.0'C

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  RGB_NUM = 47;
  RGB_init(RGB_NUM);
}

void loop() {
  if(Serial.available()) {
    Serial.println("ssibal");
    // char c = Serial.read();
    color += Serial.parseInt();
    Serial.println(color);
    return;
    delay(10);
  }
  Serial.flush();
  Serial.println(color);
  temper_pid.DEBUG();
  ld ans = temper_pid.pid_control(50, 30);
  Serial.println(ans);

  delay(200);
}
