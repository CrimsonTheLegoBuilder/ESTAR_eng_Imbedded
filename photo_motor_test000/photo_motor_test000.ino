const int sensorPin = 2;
const int motorPin = 8;
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm = 0;
const int teethCount = 23;
volatile int cnt = 0;
volatile bool directionChanged = false;
volatile int dir = 0;
const unsigned long debounceDelay = 200;

void onPulse() {
  unsigned long currentPulseTime = micros();
  if (currentPulseTime - lastPulseTime >= debounceDelay) { // 디바운싱 처리
    pulseInterval = (currentPulseTime - lastPulseTime);
    lastPulseTime = currentPulseTime;
    // cnt++;
    // Serial.print("cnt++:: ");
    Serial.println(cnt);
    if (cnt >= teethCount) {
      directionChanged = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, dir);
  attachInterrupt(digitalPinToInterrupt(sensorPin), onPulse, RISING);
}

void loop() {
  if (pulseInterval > 0) {
    rpm = (1000000.0 / pulseInterval) * (60.0 / teethCount);
    Serial.print("RPM: ");
    Serial.println(rpm);
    pulseInterval = 0;
    cnt++;
    Serial.print("cnt++:: ");
    Serial.println(cnt);
  }

  if (directionChanged) {
    directionChanged = false;
    cnt = 0;
    dir = !dir;
    digitalWrite(motorPin, dir);
    Serial.print("Direction changed to: ");
    Serial.println(dir ? "Backward" : "Forward");
  }

  int sensorState = digitalRead(sensorPin);
  Serial.print("Sensor Pin State: ");
  Serial.println(sensorState);

  delay(100);
}