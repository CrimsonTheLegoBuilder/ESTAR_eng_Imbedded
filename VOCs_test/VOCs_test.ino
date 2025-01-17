#define VOCS 1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  //VOCs
  int vocs = analogRead(VOCS);
  Serial.print("vocs: ");
  Serial.println(vocs);
  }