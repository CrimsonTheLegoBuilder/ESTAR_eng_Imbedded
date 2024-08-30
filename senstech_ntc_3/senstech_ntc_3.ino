/*
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("black: ");
  Serial.println(analogRead(0));
  Serial.print("  red: ");
  Serial.println(analogRead(1));
  Serial.print("white: ");
  Serial.println(analogRead(2));
  delay(1000);
}
*/
int ThermistorPin = 0;
int V0, V1, V2;
float R25 = 10000;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
}

float cal_the(int V) {
  float R = R25 * (1023.0 / (float)V - 1.0);
  float logR = log(R);
  float T = (1.0 / (c1 + c2*logR + c3*logR*logR*logR));
  T = T - 273.15;
  return T;
}

void loop() {
  V0 = analogRead(0);
  V1 = analogRead(1);
  V2 = analogRead(2);
  Serial.print("Temperature black: "); 
  Serial.print(cal_the(V0));
  Serial.println(" 'C");
    Serial.print("Temperature   red: "); 
  Serial.print(cal_the(V1));
  Serial.println(" 'C"); 
    Serial.print("Temperature white: "); 
  Serial.print(cal_the(V2));
  Serial.println(" 'C"); 

  delay(1000);
}
