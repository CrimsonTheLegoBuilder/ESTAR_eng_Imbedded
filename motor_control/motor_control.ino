/*
*/

#include "systemConfig.h"
// #include "motorControl.h"

// Motor m2;
unsigned long prev;

int dir = 1;
int ang = 90;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR2_DIR1, OUTPUT);
  pinMode(MOTOR2_DIR2, OUTPUT);
  pinMode(MOTOR2_SPD, OUTPUT);
  // m2.init();
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(BTN4_PIN, INPUT_PULLUP);
  Serial.println(digitalRead(BTN3_PIN));
  Serial.println(digitalRead(BTN4_PIN));
  // while (1) {
  //   int x3 = digitalRead(BTN3_PIN);
  //   int x4 = digitalRead(BTN4_PIN);
  //   if (x3 || x4) break;
  // }
  // prev = millis();
}

void loop() {
  int x3 = digitalRead(BTN3_PIN);
  int x4 = digitalRead(BTN4_PIN);
  // Serial.print("x3: ");
  Serial.println(x3);
  // delay(100);
  Serial.println(x4);
  if (x3 || x4) {
    digitalWrite(MOTOR2_DIR1, 0); 
    digitalWrite(MOTOR2_DIR2, 0); 
    analogWrite(MOTOR2_SPD, 0);
    delay(100);
    dir++;
    dir %= 2;
    digitalWrite(MOTOR2_DIR1, dir);
    digitalWrite(MOTOR2_DIR2, (dir + 1) % 2);
    analogWrite(MOTOR2_SPD, 150);
    delay(1000);
  }
  Serial.println(dir);
  digitalWrite(MOTOR2_DIR1, dir);
  digitalWrite(MOTOR2_DIR2, (dir + 1) % 2);
  analogWrite(MOTOR2_SPD, 150);
  // delay(1000);


  // digitalWrite(MOTOR2_DIR1, 1);
  // digitalWrite(MOTOR2_DIR2, 0);
  // analogWrite(MOTOR2_SPD, 100);
  // delay(3000);
  // digitalWrite(MOTOR2_DIR1, 0); 
  // digitalWrite(MOTOR2_DIR2, 0); 
  // analogWrite(MOTOR2_SPD, 0);
  // delay(100);
  // digitalWrite(MOTOR2_DIR1, 0); 
  // digitalWrite(MOTOR2_DIR2, 1); 
  // analogWrite(MOTOR2_SPD, 100);
  // delay(3000);
}
