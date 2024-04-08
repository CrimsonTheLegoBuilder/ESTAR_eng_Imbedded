// #include <SoftwareSerial.h>
// #include "_config.h"
#include "ledControl.h"
#include "dhtControl.h"
#include "pidControl.h"
#include "ssrControl.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "DHT_Async.h"
#define DHT_PIN 3
#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_Async dht_sensor(DHT_PIN, DHT_TYPE_11);

typedef long long ll;
typedef double ld;

/*

Default config::
  RGB_PIN 2
  DHT_PIN 3

*/

extern int __bss_end;
extern void *__brkval;

int memoryPrint() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}


int ledPin = 11;
extern int color;
float setPoint = 0, tempValue = 0, humiValue = 0;
PID temper_pid = PID();
bool t_valid = 0;

SemaphoreHandle_t xSemaphore = NULL;
// SemaphoreHandle_t xCountingSemaphore;

// void TaskDhtRead(void *pvParameters);
// void TaskPidControll(void *pvParameters);
void setup() {
  Serial.begin(9600);
  // pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(ledPin, OUTPUT);

  dht::init();
  // delay(5);
  // humidity & temper sensor config
  rgb::init(50);
  // delay(50);/
  // led strip length config

  // ssr::init(8, 9, 10);

  temper_pid.init(.2, .1, .0005, 1e-4, .0);
  // // pid parameters setting
  // // pid.init(Kp, Ki, Kd, dt, low_limit, high_limit);
  // // high_limit's default value = 70.0'C
  setPoint = 50.0;

  t_valid = 0;

  // xSemaphore = xSemaphoreCreateMutex();
  // if (xSemaphore != NULL) {
  //   Serial.println("Mutex created");
  // }
  if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  Serial.println("fuck");
  Serial.println(xTaskCreate(
    TaskDhtRead,
    "DhtRead",
    350,
    NULL,
    2,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskPidControll,
    "PidControll",
    250,
    NULL,
    1,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskRgbControll,
    "RgbControll",
    200,
    NULL,
    0,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskSsrControll,
    "SsrControll",
    200,
    NULL,
    2,
    NULL
  ));

  Serial.println("fuck");
  Serial.println(memoryPrint());
  // vTaskStartScheduler();
  // xCountingSemaphore = xSemaphoreCreateCounting(1,1);
  // xSemaphoreGive(xCountingSemaphore);
}

void loop() {
    // Data humitmp = dht::check();
    // tempValue = humitmp.temp;
    // Serial.println(tempValue);
}

void TaskDhtRead(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    TickType_t xS, xE, xExc;
    xS = xTaskGetTickCount();
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      //Serial.println("Inside Task1 and Serial monitor Resource Taken");
      // Data humitmp = dht::check();
      // tempValue = humitmp.temp;
      t_valid = dht_sensor.measure(&tempValue, &humiValue);
      Serial.print("t_valid : ");
      Serial.println(t_valid);
      Serial.print("tempValue : ");
      Serial.println(tempValue);
      Serial.println(memoryPrint());
      xSemaphoreGive(xSemaphore);
    }
    xE = xTaskGetTickCount();
    xExc = xE - xS;
    // if (xExc < xFreq) vTaskDelayUntil(&xLastWakeTime, xFreq - xExc);
    // else xLastWakeTime = xE;
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    //vTaskDelay(15);
  }
}

void TaskPidControll(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    TickType_t xS, xE, xExc;
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      //Serial.println("Inside Task2 and Serial monitor Resource Taken");
      if (t_valid) {
        float t = temper_pid.pid_control(setPoint, tempValue);
        // Serial.println(temper_pid.pid_control(setPoint, tempValue));
        // bool f = dht_sensor.measure(&tempValue, &humiValue);
        Serial.println(t);
      }
      Serial.println(memoryPrint());
      xSemaphoreGive(xSemaphore);
    }
    xE = xTaskGetTickCount();
    xExc = xE - xS;
    // if (xExc < xFreq) vTaskDelayUntil(&xLastWakeTime, xFreq - xExc);
    // else xLastWakeTime = xE;
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    //vTaskDelay(15);
  }
}

void TaskRgbControll(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(500);
  xLastWakeTime = xTaskGetTickCount();
  int col = 1;
  for (;;) {
    col = (col + 1) % 6;
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      //Serial.println("Inside Task3 and Serial monitor Resource Taken");
      rgb::fixed(col);
      xSemaphoreGive(xSemaphore);
    }
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void TaskSsrControll(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      Serial.println("Inside Task4 and Serial monitor Resource Taken");
      // ssr::fan_control(50);
      // ssr::ptc_control(50);
      xSemaphoreGive(xSemaphore);
    }
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
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
