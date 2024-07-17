const int en1 = 5;
const int in1 = 6;
const int in2 = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(en1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(en1, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  delay(2000);
}
