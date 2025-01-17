#define TVOC 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5, INPUT);
  pinMode(6, INPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  // int hi = digitalRead(5);
  // int lo = digitalRead(6);
  // Serial.print(hi);
  // Serial.print(" ");
  // Serial.println(lo);
  // delay(1000);
  unsigned long duration = pulseInLong(5, HIGH);
  Serial.println(duration);
  delay(500);
}
