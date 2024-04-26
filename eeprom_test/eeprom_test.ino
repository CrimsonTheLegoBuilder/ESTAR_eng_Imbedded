const byte byteLists[] PROGMEM = { 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000 };
const char charLists[] PROGMEM = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
const uint16_t intLists[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6 };

byte bTemp;
char cTemp;
int iTemp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("program Start!");
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 7; i++) {
    bTemp = pgm_read_byte_near(&(byteLists[i]));
    cTemp = pgm_read_byte_near(&(charLists[i]));
    iTemp = pgm_read_byte_near(&(intLists[i]));
    Serial.print("0b");
    Serial.println(bTemp, BIN);
    Serial.println(cTemp);
    Serial.println(iTemp);
    delay(500);
  }
}
