#define ECHO 7

//Mux control pins
int s0 = 8;
int s1 = 9;
int s2 = 10;
int s3 = 11;

//Mux in “SIG” pin
int SIG_pin = 3;

void setup() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(SIG_pin, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ECHO - 1, INPUT);
  pinMode(ECHO - 2, INPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  digitalWrite(SIG_pin, LOW);

  Serial.begin(9600);
}

void loop() {
//Loop through and read all 16 values
//Reports back Value at channel 6 is: 346
  for(int i = 0; i < 3; i++){ 
    Serial.print("Value at channel "); 
    Serial.print(i);
    Serial.print(": "); 
    Serial.println(detect(i));
    delay(100);
  }
}

void select_sensor(int channel) { 
  int controlPin[] = {s0, s1, s2, s3};
  for (int i = 0; i < 3; i++){
    digitalWrite(controlPin[i], !!(channel & (1 << i))); 
    //Serial.print(!!(channel & (1 << i)));
  }
  return; 
}

float detect(int trig) {
  select_sensor(trig);
  long duration, distance;
  digitalWrite(SIG_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(SIG_pin, HIGH);
  delayMicroseconds(20); 
  digitalWrite(SIG_pin, LOW);
  duration = pulseIn(ECHO, HIGH);
  // duration = pulseIn(trig + 5, HIGH);
  distance = duration * 17 / 1000; 

  delayMicroseconds(20);
  return distance;
}

// #define TRIG1 2
// #define TRIG2 3
// #define ECHO1 4
// #define ECHO2 5
//   long duration, distance;

// void setup() {
//   pinMode(TRIG1, OUTPUT);
//   pinMode(TRIG2, OUTPUT);
//   pinMode(ECHO1, INPUT);
//   pinMode(ECHO2, INPUT);
//   Serial.begin(9600);
// }

// void loop() {
//   detect(TRIG1, ECHO1);
//   Serial.print("distance 1 : ");
//   Serial.println(distance);
//   detect(TRIG2, ECHO2);
//   Serial.print("distance 2 : ");
//   Serial.println(distance);
//   delay(500);
// }

// void detect(int trig, int echo) {
//   digitalWrite(trig, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trig, HIGH);
//   delayMicroseconds(20);
//   digitalWrite(trig, LOW);
//   duration = pulseIn(echo, HIGH);
//   distance = duration * 17 / 1000; 
//   Serial.println(duration);
// }
