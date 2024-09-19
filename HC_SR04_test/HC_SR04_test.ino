// #define echoPin 11 // attach pin D2 Arduino to pin Echo of JSN-SR04T
// #define trigPin 12 //attach pin D3 Arduino to pin Trig of JSN-SR04T

// // defines variables
// long duration; // variable for the duration of sound wave travel
// int distance; // variable for the distance measurement

// void setup() {
//   pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
//   pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
//   Serial.begin(115200); // // Serial Communication is starting with 9600 of baud rate speed
//   Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
//   Serial.println("with Arduino UNO R3");
// }
// void loop() {
//   // Clears the trigPin condition
//   digitalWrite(trigPin, LOW);  //
//   delayMicroseconds(2);
//   // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   // Reads the echoPin, returns the sound wave travel time in microseconds
//   duration = pulseIn(echoPin, HIGH);
//   // Calculating the distance
//   distance = duration / 29 / 2; // Speed of sound wave divided by 2 (go and back)
//   // Displays the distance on the Serial Monitor
//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");// working  code for aj-sr04m
// }

#include <SoftwareSerial.h>
#define rxPin 11
#define txPin 12
 
SoftwareSerial jsnSerial(rxPin, txPin);
 
void setup() {
  jsnSerial.begin(9600);
  Serial.begin(115200);
}
 
void loop() {
  /*jsnSerial.write(0x01);
   delay(10);*/
  if(jsnSerial.available()){
    getDistance();
   //
  
  }
  
}
void getDistance(){
  unsigned int distance;
  byte startByte, h_data, l_data, sum = 0;
  byte buf[5];
  if (jsnSerial.available()) {
    jsnSerial.readBytes(buf, 4);
    Serial.print(buf[0], HEX);
    Serial.print(" ");
    Serial.print(buf[1], HEX);
    Serial.print(" ");
    Serial.print(buf[2], HEX);
    Serial.print(" ");
    Serial.print(buf[3], HEX);
    Serial.print(" ");
    Serial.print(buf[4], HEX);
    Serial.println(" ");
    h_data = buf[1];
    l_data = buf[2];
    sum = buf[3];
    distance = (h_data<<8) + l_data;
    Serial.print("Distance [mm]: "); 
    Serial.println(distance);
    if((( h_data + l_data) & 0xFF) != sum){
      Serial.println("Invalid result");
    }
    else{
      Serial.print("Distance [mm]: "); 
      Serial.println(distance);
    } 
  }
  //startByte = (byte)jsnSerial.read();
  //Serial.println(startByte);
  // if(startByte == 255){
  //   jsnSerial.readBytes(buf, 5);
  //   h_data = buf[0];
  //   l_data = buf[1];
  //   sum = buf[2];
  //   distance = (h_data<<8) + l_data;
  //   Serial.print(buf[0], HEX);
  //   Serial.print(" ");
  //   Serial.print(buf[1], HEX);
  //   Serial.print(" ");
  //   Serial.print(buf[2], HEX);
  //   Serial.print(" ");
  //   Serial.print(buf[3], HEX);
  //   Serial.print(" ");
  //   Serial.print(buf[4], HEX);
  //   Serial.println(" ");
    // if((( h_data + l_data)) != sum){
    //   Serial.println("Invalid result");
    // }
    // else{
    //   Serial.print("Distance [mm]: "); 
    //   Serial.println(distance);
    // } 
  //} 
  //else return;
  delay(100);
}