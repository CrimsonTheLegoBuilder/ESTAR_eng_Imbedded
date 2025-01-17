volatile unsigned long ledStartTime = 0;
volatile unsigned long ledPulseWidth = 0;
volatile unsigned long laserStartTime = 0;
volatile unsigned long laserPulseWidth = 0;
volatile unsigned long signalStartTime = 0;
volatile unsigned long signalPulseWidth = 0;
volatile int ledMode = 0;
volatile int laserMode = 0;
volatile int signalMode = 0;

void ISR_LED() {
  if (digitalRead(2) == HIGH) {
    ledStartTime = micros();
  }
  else {
    ledPulseWidth = micros() - ledStartTime;
    if (1450 <= ledPulseWidth && ledPulseWidth <= 1650) ledMode = 0;
    else if (ledPulseWidth < 1450) ledMode = -1;
    else if (1650 < ledPulseWidth) ledMode = 1;
  }
}

void ISR_LASER() {
  if (digitalRead(3) == HIGH) {
    laserStartTime = micros();
  }
  else {
    laserPulseWidth = micros() - laserStartTime;
    if (laserPulseWidth <= 1450) laserMode = 0;
    else if (1450 < laserPulseWidth) laserMode = 1;
  }
}

// void ISR_SIGNAL() {
//   if (digitalRead(4) == HIGH) {
//     signalStartTime = micros();
//   }
//   else {
//     signalPulseWidth = micros() - signalStartTime;
//     if (signalPulseWidth <= 1450) signalMode = 0;
//     else if (1450 < signalPulseWidth) signalMode = 1;
//   }
// }

// void timerCallback() {
//   if (pulseUpdated) {
//     Serial.print("Pulse Width: ");
//     Serial.print(pulseWidth);
//     Serial.println(" µs");
//     pulseUpdated = 0;
//   }
// }

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), ISR_LED, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), ISR_LASER, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(4), ISR_SIGNAL, CHANGE);
  Serial.begin(115200);
  digitalWrite(5, 1);
  digitalWrite(6, 1);
  digitalWrite(7, 1);
  digitalWrite(8, 1);
}

//int t = 0;
void loop() {
  if (ledMode == 0) {
    digitalWrite(5, 0);
    digitalWrite(6, 0);
  }
  else if (ledMode == -1) {
    digitalWrite(5, 1);
    digitalWrite(6, 0);
  }
  else if (ledMode == 1) {
    digitalWrite(5, 0);
    digitalWrite(6, 1);
  }

  digitalWrite(7, laserMode);

  // if (t == 9) {
    signalPulseWidth = pulseIn(4, HIGH);
    signalMode = signalPulseWidth >= 1600 ? 0 : 1;
    digitalWrite(8, signalMode);
    Serial.println(signalPulseWidth);
  // }
  //t = (t + 1) % 10;
  Serial.println(signalMode);

  delay(100);
}