void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, 100);
  // int x = analogRead(A8);
  int x = digitalRead(12);
  Serial.println(x);
  delay(1000);
}
