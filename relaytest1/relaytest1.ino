int RelayPin4 = 4; 

void setup() {
  // put your setup code here, to run once:
  pinMode(RelayPin4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RelayPin4, LOW);
  Serial.println("LOW");
  delay(1000);
  digitalWrite(RelayPin4, HIGH);
  Serial.println("HIGH");
  delay(1000);
}
