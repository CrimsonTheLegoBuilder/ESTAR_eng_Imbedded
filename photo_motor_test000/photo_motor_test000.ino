const int sensorPin = 2; // 인터럽트를 설정할 핀 번호
const int motorPin = 8;
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm = 0;
const int teethCount = 23;
volatile int cnt = 0;
volatile bool directionChanged = false;
volatile int dir = 0;
const unsigned long debounceDelay = 500; // 디바운싱을 위한 지연 시간 (마이크로초)

void onPulse() {
  unsigned long currentPulseTime = micros();
  if (currentPulseTime - lastPulseTime >= debounceDelay) { // 디바운싱 처리
    pulseInterval = (currentPulseTime - lastPulseTime);
    lastPulseTime = currentPulseTime;
    cnt++;
    Serial.print("cnt++:: ");
    Serial.println(cnt);
    if (cnt >= teethCount) {
      cnt = 0;
      dir = !dir;
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
    pulseInterval = 0; // 펄스 간격 초기화
  }

  // 방향 변경을 메인 루프에서 처리
  if (directionChanged) {
    directionChanged = false;
    digitalWrite(motorPin, dir);
    Serial.print("Direction changed to: ");
    Serial.println(dir ? "Backward" : "Forward");
  }

  // 센서 핀의 상태 디버깅
  int sensorState = digitalRead(sensorPin);
  Serial.print("Sensor Pin State: ");
  Serial.println(sensorState);

  delay(500); // 0.5초마다 상태 출력
}