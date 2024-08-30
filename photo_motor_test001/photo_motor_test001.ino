const int sensorPin = 2; // 인터럽트를 설정할 핀 번호
const int motorPin = 8;
const int btnPin = 3;
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm = 0;
const int teethCount = 23;
volatile int cnt = 0;
volatile bool directionChanged = false;
volatile int dir = 0;//0 = CW, 1 = CCW
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // 디바운싱을 위한 지연 시간 (마이크로초)
const unsigned long btnDebounce = 50; // 디바운싱을 위한 지연 시간 (마이크로초)
volatile bool buttonState = HIGH; // 버튼의 현재 상태
volatile bool lastButtonState = HIGH; // 마지막으로 읽은 버튼의 상태

void onPulse() {
  unsigned long currentPulseTime = micros();
  if (currentPulseTime - lastPulseTime >= debounceDelay) { // 디바운싱 처리
    pulseInterval = (currentPulseTime - lastPulseTime);
    lastPulseTime = currentPulseTime;
    if (!dir) return;
    // cnt++;
    // Serial.print("cnt++:: ");
    // Serial.println(cnt);
    if (cnt >= teethCount) {
      //cnt = 0;
      //dir = !dir;
      directionChanged = true;
    }
  }
}

void btnPulse() {
  cnt = 0;
  directionChanged = true;
}

// void btnPulse() {
//   unsigned long currentTime = micros();
//   if (currentTime - lastDebounceTime > btnDebounce) {
//     // 버튼 상태 변경
//     buttonState = digitalRead(btnPin);
//     // 버튼 상태가 바뀌었을 때만 처리
//     if (buttonState == LOW && lastButtonState == HIGH) {
//       // 버튼이 눌렸을 때 처리할 코드
//       directionChanged = true;
//       Serial.println("Button pressed");
//     }
//     // 마지막 버튼 상태 업데이트
//     lastButtonState = buttonState;
//     lastDebounceTime = currentTime;
//   }
// }

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), onPulse, RISING);
  attachInterrupt(digitalPinToInterrupt(btnPin), btnPulse, FALLING);
  /*
  눌려있는 상황
  else 
  */
  dir = 0;
  digitalWrite(motorPin, dir);
}

void loop() {
  // if (pulseInterval > 0) {
  //   rpm = (1000000.0 / pulseInterval) * (60.0 / teethCount);
  //   Serial.print("RPM: ");
  //   Serial.println(rpm);
  //   pulseInterval = 0; // 펄스 간격 초기화
  //   if (dir) {
  //     cnt++;
  //     Serial.print("cnt++:: ");
  //     Serial.println(cnt);
  //   }
  // }

  // // 방향 변경을 메인 루프에서 처리
  // if (directionChanged) {
  //   dir = !dir;
  //   cnt = 0;
  //   directionChanged = false;
  //   digitalWrite(motorPin, dir);
  //   Serial.print("Direction changed to: ");
  //   Serial.println(dir ? "Backward" : "Forward");
  // }

  // 센서 핀의 상태 디버깅
  int sensorState = digitalRead(sensorPin);
  Serial.print("Sensor Pin State: ");
  Serial.println(sensorState);
  int btnState = digitalRead(btnPin);
  Serial.print("Btn Pin State: ");
  Serial.println(btnState);

  delay(10); // 0.5초마다 상태 출력
}