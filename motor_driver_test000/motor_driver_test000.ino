#include "boardconfig.h"

int speed, acc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  speed = 125;
  acc = 25;
  pinMode(MOTOR_1_DIR, OUTPUT);
  pinMode(MOTOR_2_DIR, OUTPUT);
  ledcAttach(MOTOR_1_PWM, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(MOTOR_2_PWM, PWM_FREQUENCY, PWM_RESOLUTION);
}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MOTOR_1_DIR, 1);
  digitalWrite(MOTOR_2_DIR, 1);
  ledcWrite(MOTOR_1_PWM, speed);
  ledcWrite(MOTOR_2_PWM, speed);
  Serial.println(speed);
  delay(1000);
}
