const int sensorPin = 2; // 인터럽트를 설정할 핀 번호
const int motorSpeed = 8;
const int motorDir1 = 9;
const int motorDir2 = 10;
const int btnPin = 3;
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm = 0;
int speed = 0;
const int teethCount = 23;
volatile int cnt = 0;
volatile bool directionChanged = false;
volatile int dir = 0;//0 = CW, 1 = CCW
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // 디바운싱을 위한 지연 시간 (마이크로초)

void onPulse() {
  unsigned long currentPulseTime = micros();
  if (currentPulseTime - lastPulseTime >= debounceDelay) { // 디바운싱 처리
    pulseInterval = (currentPulseTime - lastPulseTime);
    lastPulseTime = currentPulseTime;
    //if (!dir) return;
    if (cnt >= teethCount) {
      directionChanged = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  speed = 0;
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(motorSpeed, OUTPUT);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), onPulse, RISING);
  dir = 1;
  analogWrite(motorSpeed, speed);
  digitalWrite(motorDir1, dir);
  digitalWrite(motorDir2, !dir);
  directionChanged = false;
}

void loop() {
  Serial.println(speed);
  Serial.println(directionChanged);
  if (directionChanged == false && speed < 240) {
    Serial.println("speedUP");
    speed += 10;
    analogWrite(motorSpeed, speed);
    delay(100);
    return;
  }

  if (directionChanged == true && speed > 10) {
    Serial.println("speedDOWN");
    speed -= 10;
    analogWrite(motorSpeed, speed);
    delay(100);
    return;
  }

  if (pulseInterval > 0) {
    rpm = (1000000.0 / pulseInterval) * (60.0 / teethCount);
    Serial.print("RPM: ");
    Serial.println(rpm);
    pulseInterval = 0; // 펄스 간격 초기화
    // if (dir) {
    //   cnt++;
    //   Serial.print("cnt++:: ");
    //   Serial.println(cnt);
    // }
          cnt++;
      Serial.print("cnt++:: ");
      Serial.println(cnt);
  }

  // 방향 변경을 메인 루프에서 처리
  if (directionChanged) {
    dir = !dir;
    cnt = 0;
    directionChanged = false;
    digitalWrite(motorDir1, dir);
    digitalWrite(motorDir2, !dir);
    Serial.print("Direction changed to: ");
    Serial.println(dir ? "Backward" : "Forward");
  }

  // 센서 핀의 상태 디버깅
  int sensorState = digitalRead(sensorPin);
  Serial.print("Sensor Pin State: ");
  Serial.println(sensorState);
  int btnState = digitalRead(btnPin);
  Serial.print("Btn Pin State: ");
  Serial.println(btnState);

  delay(100); // 0.5초마다 상태 출력
}