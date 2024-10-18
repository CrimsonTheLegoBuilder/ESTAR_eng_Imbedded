#ifndef _SPRAY_SOURCE_
#define _SPRAY_SOURCE_

void Spray::init() {
  pinMode(pump_pin, OUTPUT);
  pinMode(air_sol_pin, OUTPUT);
  pinMode(water_sol_pin, OUTPUT);
  pinMode(pressure_pin1, INPUT);
}
void Spray::change_pressure_limit(int hi, int lo) {
  prs_hi = hi;
  prs_lo = lo;
}
void Spray::shut_off() {
  pump_state = LOW;
  digitalWrite(pump_pin, LOW);
  air_state = LOW;
  digitalWrite(air_sol_pin, LOW);
  water_state = LOW;
  digitalWrite(water_state, LOW);
}
void Spray::pump_on() {
  pump_state = HIGH;
  digitalWrite(pump_pin, HIGH);
}
void Spray::pump_off() {
  pump_state = LOW;
  digitalWrite(pump_pin, LOW);
}
void Spray::air_on() {
  air_state = HIGH;
  digitalWrite(air_sol_pin, HIGH);
}
void Spray::air_off() {
  air_state = LOW;
  digitalWrite(air_sol_pin, LOW);
}
void Spray::water_on() {
  water_state = HIGH;
  digitalWrite(water_state, HIGH);
}
void Spray::water_off() {
  water_state = LOW;
  digitalWrite(water_state, LOW);
}
int Spray::pressure_read() {
  int v = analogRead(pressure_pin1);
  int p = map(v, int(4096 * 0.2), 4096, 100, 1000);
  return p;
}


#endif