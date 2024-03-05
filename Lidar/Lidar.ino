// LD19 PWM_FREQ MIN:20kHz TYP:30kHz 50kHz
// PWM 0% 40% 100% 40%=10HZ
// LD19: 230400 bit/s
//
// LD14: 115200 bit/s


int LD_TYPE   = 0;
int LD14_FAST = 0;
int LD19_FAST = 0;
int WIRE_CTRL = 1;

#define SWITCH_PIN 12
#define LD14_PIN   13
#define LD19_PIN   14

#define START_PIN  5

#define PWM_PIN  4


void setup() {
  // put your setup code here, to run once:
  pinMode(SWITCH_PIN,INPUT);
  pinMode(LD14_PIN,INPUT);
  pinMode(LD19_PIN,INPUT);

  pinMode(START_PIN,OUTPUT);
  
  digitalWrite(START_PIN, HIGH);

//  analogWriteFreq(24000);
//  analogWriteRange(1023);
//  analogWrite(PWM_PIN, 480);

  Serial.begin(115200);
}


void switchCheck(){
  if(digitalRead(SWITCH_PIN) == LOW){
    digitalWrite(START_PIN, LOW);
    WIRE_CTRL = 1;
  }
  else{
    digitalWrite(START_PIN, HIGH);
  }
}


//void speedCheck(){
//  if(digitalRead(LD19_PIN) == LOW){
//    analogWrite(PWM_PIN, 1020);
//  }
//  else{
//    analogWrite(PWM_PIN, 480);
//  }
//  
//  if(digitalRead(LD14_PIN) == LOW){
//    analogWrite(PWM_PIN, 550);
//  }
//  else{
//    analogWrite(PWM_PIN, 480);
//  }
//}


void loop() {
  // put your main code here, to run repeatedly:
  if(WIRE_CTRL == 1){
    switchCheck();
  }
  
  if (Serial.available() > 0) {
    char state = Serial.read();
    if (state == '1') {
      digitalWrite(START_PIN, HIGH);
      WIRE_CTRL = 0;
    }
    else if (state == '0') {
      digitalWrite(START_PIN, LOW);
      WIRE_CTRL = 0;
    }
    else if (state == '2') {
      WIRE_CTRL = 1;
    }
  }

}
