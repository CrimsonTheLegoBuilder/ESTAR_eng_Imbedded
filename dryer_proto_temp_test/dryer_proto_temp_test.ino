#include <Arduino.h>
#include <stdio.h>
#include <math.h>
// #include <SoftwareSerial.h>
// #include "_config.h"
// #include "ledControl.h"
// #include "dhtControl.h"
// #include "pidControl.h"

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

int ledPin = 11;
extern int color;
float setPoint = 0, tempValue = 0, humiValue = 0;
// PID temper_pid = PID();

SemaphoreHandle_t xSemaphore = NULL;
// SemaphoreHandle_t xCountingSemaphore;

// void TaskDhtRead(void *pvParameters);
// void TaskPidControll(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // dht::init();
  // humidity & temper sensor config

  // rgb::init(47);
  // // led strip length config

  // temper_pid.init(.2, .1, .05, 1e-4, .0);
  // // pid parameters setting
  // // pid.init(Kp, Ki, Kd, dt, low_limit, high_limit);
  // // high_limit's default value = 70.0'C
  // setPoint = 50.0;

  // xSemaphore = xSemaphoreCreateMutex();
  // if (xSemaphore != NULL) {
  //   Serial.println("Mutex created");
  // }
  if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  xTaskCreate(
    TaskDhtRead,
    "DhtRead",
    400,
    NULL,
    0,
    NULL
  );

  xTaskCreate(
    TaskPidControll,
    "PidControll",
    400,
    NULL,
    2,
    NULL
  );

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
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)5) == pdTRUE) {
      Serial.println("Inside Task1 and Serial monitor Resource Taken");
      // Data humitmp = dht::check();
      // tempValue = humitmp.temp;
      bool f = dht_sensor.measure(&tempValue, &humiValue);
      Serial.println(tempValue);
      xSemaphoreGive(xSemaphore);
    }
    vTaskDelay(15);
  }
}

void TaskPidControll(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)5) == pdTRUE) {
      Serial.println("Inside Task2 and Serial monitor Resource Taken");
      // Serial.println(temper_pid.pid_control(setPoint, tempValue));
      xSemaphoreGive(xSemaphore);
    }
    vTaskDelay(15);
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
