int pinNum[] = { 2, 3, 4, 8, 9, 10 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(pinNum[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(0);
  float prs = (x - 206) * 1500 / 817;
  Serial.println(prs);
  delay(1000);
}
