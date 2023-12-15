typedef double ld;

ld err, err_prev;
ld Kp = 0.3;
ld Ki = 0.05;
ld Kd = 0.005;

ld P, I, D, cur;
ld PID;
ld term = 0.05;
ld target = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  PID_control();
}

void PID_control() {
  cur = analogRead(A0);
  cur = map(cur, 0, 255, 255, 0);
  Serial.println(cur);//DEBUG

  err = target - cur;
  P = Kp * err;
  I = I + Ki * err * term;
  D = Kd * (err - err_prev) / term;
  PID = P + I + D;
  PID = constrain(PID, 0, 255);
  // Serial.println(PID);
  analogWrite(6, PID);
  err_prev = err;
}


  // if (readValue > 120)
  //   digitalWrite(13, HIGH);
  // else
  //   digitalWrite(13, LOW);
  // delay(50);