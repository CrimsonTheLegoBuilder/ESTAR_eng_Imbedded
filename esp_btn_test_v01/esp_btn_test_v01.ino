#define BTN_PIN 32
#define PHOTO_PIN 25
#define LEVEL_PIN 33
#define PRESSURE_PIN 25

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // pinMode(BTN_PIN, INPUT);
  // pinMode(LEVEL_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // int x = digitalRead(LEVEL_PIN);
  int x = analogRead(PRESSURE_PIN);
  Serial.println(x);
  delay(1000);
}
