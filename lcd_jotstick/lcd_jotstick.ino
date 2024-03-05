#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int lx = A0;
int ly = A1;
int lz = 7;
int rx = A2;
int ry = A3;
int rz = 6;
int LX, LY, RX, RY;

void setup() {
  Serial.begin(9600);
  pinMode(lx, INPUT);
  pinMode(ly, INPUT);
  pinMode(lz, INPUT_PULLUP);
  pinMode(rx, INPUT);
  pinMode(ry, INPUT);
  pinMode(rz, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  LX = 0; LY = 0;
  RX = 15; RY = 1;
}

void loop() {
  lcd.clear();
  lcd.setCursor(LX, LY);
  lcd.print("L");
  lcd.setCursor(RX, RY);
  lcd.print("R");
  int dlx = analogRead(lx);
  int dly = analogRead(ly);
  int init_l = digitalRead(lz);
  int drx = analogRead(rx);
  int dry = analogRead(ry);
  int init_r = digitalRead(rz);
  Serial.print(dlx);
  Serial.print(" ");
  Serial.print(dly);
  Serial.print(" ");
  Serial.print(init_l);
  Serial.print(" ");
  Serial.print(drx);
  Serial.print(" ");
  Serial.print(dry);
  Serial.print(" ");
  Serial.println(init_r);
  if (LX < 15 && dlx < 350) LX++;
  else if (LX > 0 && dlx > 650) LX--;
  if (LY < 1 && dly > 650) LY++;
  else if (LY > 0 && dly < 350) LY--;
  if (!init_l) LX = 0, LY = 0;
  if (RX < 15 && drx < 350) RX++;
  else if (RX > 0 && drx > 650) RX--;
  if (RY < 1 && dry > 650) RY++;
  else if (RY > 0 && dry < 350) RY--;
  if (!init_r) RX = 15, RY = 1;
  delay(100);
}
