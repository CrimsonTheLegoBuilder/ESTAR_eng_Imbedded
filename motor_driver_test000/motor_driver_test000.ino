int pinNum[] = { 2, 3, 4, 8, 9, 10 };
int speed, acc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(pinNum[i], OUTPUT);
  }
  speed = 250;
  acc = 25;
  analogWrite(2, speed);
  analogWrite(8, speed);
  digitalWrite(3, 0);
  digitalWrite(4, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
}


void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(2, speed);
  analogWrite(8, speed);
  // speed += acc;
  // if (acc > 0 && speed > 250) {
    // acc *= -1;
    // speed += acc;
  // }
  // else if (acc < 0 && speed < 0) {
    // acc *= -1;
    // speed += acc;
  // }
  Serial.println(speed);
  delay(1000);
}
