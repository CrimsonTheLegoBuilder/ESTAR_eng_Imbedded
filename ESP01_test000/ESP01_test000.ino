//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3);  //RX, TX

void setup() {
  Serial.begin(9600);
  //mySerial.begin(74880);
}

void loop() {
  Serial.println("fuck::");
  //if (mySerial.available()) {
  //  Serial.write(mySerial.read());
  //}
  //if (Serial.available()) {
  //  mySerial.write(Serial.read());
  //}
  delay(30);
}
