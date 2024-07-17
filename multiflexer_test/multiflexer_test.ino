const int muxControlPins[] = {2, 3, 4, 5}; // 멀티플렉서 제어 핀
const int triggerPin = 6; // 멀티플렉서 COM 핀
const int echoPin = 7; // 초음파 센서의 에코 핀
const int numSensors = 3;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(muxControlPins[i], OUTPUT);
  }
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  for (int sensor = 0; sensor < numSensors; sensor++) {
    selectSensor(sensor);
    long duration = getUltrasonicDistance();
    float distance = duration * 0.034 / 2;
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.print(": ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }
}

void selectSensor(int sensor) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(muxControlPins[i], bitRead(sensor, i));
  }
}

long getUltrasonicDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration;
}