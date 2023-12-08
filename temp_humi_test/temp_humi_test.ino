#include <DHT.h>
// #include <DHT_U.h>
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int RelayPin4 = 4; 
  /*불러온 라이브러리 안에 몇번 PIN에서 데이터값이 나오는지
  설정해줘야 합니다. 아날로그 0번 PIN인 A0으로 설정했습니다.
  */
void setup() {
  Serial.begin(9600);
  /*온습도값을 PC모니터로 확인하기위해
  시리얼 통신을 설정해 줍니다. */
  dht.begin();
  pinMode(RelayPin4, OUTPUT);
  digitalWrite(RelayPin4, HIGH);
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();// 습도를 측정합니다.
  float t = dht.readTemperature();// 온도를 측정합니다.
  float f = dht.readTemperature(true);// 화씨 온도를 측정합니다.

  // 값 읽기에 오류가 있으면 오류를 출력합니다.
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 보정된 화씨 값을 가져옵니다.
  float hif = dht.computeHeatIndex(f, h);
  // 보정된 섭씨 값을 가져옵니다.
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  if (h > 50.0) digitalWrite(RelayPin4, LOW);
  else digitalWrite(RelayPin4, HIGH);
}

