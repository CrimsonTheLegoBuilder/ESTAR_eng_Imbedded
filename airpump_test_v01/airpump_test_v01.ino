#include "airpump.h" 

#define AIRPUMP 7
#define SOLVALVE1 8
#define SOLVALVE2 9

bool air, sol1, sol2;
int high, low;
bool force_shut;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(AIRPUMP, OUTPUT);
  pinMode(SOLVALVE1, OUTPUT);
  pinMode(SOLVALVE2, OUTPUT);
  force_shut = 0;
  air = 1;
  sol1 = 0;
  sol2 = 1;
  high = 350;
  low = 300;
  digitalWrite(AIRPUMP, air); 
  digitalWrite(SOLVALVE1, sol1);
  digitalWrite(SOLVALVE2, sol2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    int x = Serial.parseInt();
    if (x == 0) {
      force_shut = 1;
      air = 1;
      digitalWrite(AIRPUMP, 1);
    }
    if (x == 1) {
      sol1 ^= 1;
      digitalWrite(SOLVALVE1, sol1);
    }
    if (x == 2) {
      sol2 ^= 1;
      digitalWrite(SOLVALVE2, sol2);
    }
  }
  if (force_shut) {
    delay(1000);
    return;
  }
  Serial.print("air: ");
  Serial.print(air);
  Serial.print(" sol1: ");
  Serial.print(sol1);
  Serial.print(" sol2: ");
  Serial.println(sol2);
  int V = analogRead(0);
  Serial.print("voltage: ");
  Serial.println(V);
  float prs = V * 1.1911 - 276.68;
  Serial.print("pressure: ");
  Serial.println(prs);
  int f = pressure_check(prs, high, low);
  if (f == 1) {
    air = 0;
  }
  else if (f == -1) {
    air = 1;
  }
  digitalWrite(AIRPUMP, air);
  delay(1000);
}
