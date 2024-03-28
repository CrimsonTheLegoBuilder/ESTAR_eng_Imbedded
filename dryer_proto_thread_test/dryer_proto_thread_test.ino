#include "_config.h"
#include "ledControl.h"
#include "dhtControl.h"
#include "pidControl.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

//// RGB_PIN 2
//// DHT_PIN 3
int ledPin = 11;
extern int color;
float setPoint = 0, tempValue = 0;
PID temper_pid = PID();

// SemaphoreHandle_t xSemaphore = NULL;
SemaphoreHandle_t xCountingSemaphore;

// void TaskDhtRead(void *pvParameters);
// void TaskPidControll(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  dht::init();
  // humidity & temper sensor config

  // rgb::init(47);
  // // led strip length config

  // temper_pid.init(.2, .1, .05, 1e-4, .0);
  // // pid parameters setting
  // // pid.init(Kp, Ki, Kd, dt, low_limit, high_limit);
  // // high_limit's default value = 70.0'C
  // setPoint = 50.0;

  // xSemaphore = xSemaphoreCreateMutex();
  //   if (xSemaphore != NULL) {
  //   Serial.println("Mutex created");
  // }

  xTaskCreate(
    TaskDhtRead,
    "DhtRead",
    128,
    NULL,
    0,
    NULL
  );

  xTaskCreate(
    TaskPidControll,
    "PidControll",
    128,
    NULL,
    0,
    NULL
  );

  // vTaskStartScheduler();
  xCountingSemaphore = xSemaphoreCreateCounting(1,1);
  xSemaphoreGive(xCountingSemaphore);
}

void loop() {
    // Data humitmp = dht::check();
    // tempValue = humitmp.temp;
    // Serial.println(tempValue);
}

void TaskDhtRead(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    // if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
    //   Data humitmp = dht::check();
    //   tempValue = humitmp.temp;
    //   // Serial.println(tempValue);
    //   int w = map(tempValue, 10, 80, 0, 255);
    //   analogWrite(ledPin, w);
    //   analogWrite(ledPin, 200);
    //   xSemaphoreGive(xSemaphore);
    // }
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY); 
    Serial.println("Inside Task1 and Serial monitor Resource Taken");
    Data humitmp = dht::check();
    // Data humitmp = { 10, 60 };
    tempValue = humitmp.temp;
    Serial.println(tempValue);
    int w = map(tempValue, 0, 80, 0, 255);
    Serial.println(w);
    analogWrite(ledPin, w);
    // analogWrite(ledPin, 200);
    digitalWrite(LED_BUILTIN, HIGH);
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

void TaskPidControll(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    // if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
    //     // Serial.println(temper_pid.pid_control(setPoint, tempValue));
    //   xSemaphoreGive(xSemaphore);
    // }
    xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
    Serial.println("Inside Task2 and Serial monitor Resource Taken");
    digitalWrite(LED_BUILTIN, LOW);
    xSemaphoreGive(xCountingSemaphore);
    vTaskDelay(1);
  }
}

// void loop() {
//   if(Serial.available()) {
//     Serial.println("ssibal");
//     // char c = Serial.read();
//     color += Serial.parseInt();
//     Serial.println(color);
//     return;
//     delay(10);
//   }
//   Serial.flush();
//   Serial.println(color);
//   temper_pid.DEBUG();
//   ld ans = temper_pid.pid_control(50, 30);
//   Serial.println(ans);

//   delay(200);
// }
