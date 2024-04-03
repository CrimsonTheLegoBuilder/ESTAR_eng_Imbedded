// #include <Arduino.h>
// #include <stdio.h>
// #include <math.h>
// #include <SoftwareSerial.h>
// #include "_config.h"
// #include "ledControl.h"
// #include "dhtControl.h"
// #include "pidControl.h"

// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>

// #include "DHT_Async.h"
// #define DHT_PIN 3
// #define DHT_SENSOR_TYPE DHT_TYPE_11
// DHT_Async dht_sensor(DHT_PIN, DHT_TYPE_11);

// typedef long long ll;
// typedef double ld;

// /*

// Default config::
//   RGB_PIN 2
//   DHT_PIN 10

// */

// int ledPin = 11;
// extern int color;
// float setPoint = 0, tempValue = 0, humiValue = 0;
// PID temper_pid = PID();

// SemaphoreHandle_t xSemaphore = NULL;
// // SemaphoreHandle_t xCountingSemaphore;

// // void TaskDhtRead(void *pvParameters);
// // void TaskPidControll(void *pvParameters);

// void setup() {
//   Serial.begin(9600);
//   pinMode(LED_BUILTIN, OUTPUT);
//   pinMode(ledPin, OUTPUT);

//   // dht::init();
//   // humidity & temper sensor config

//   // rgb::init(47);
//   // // led strip length config

//   // temper_pid.init(.2, .1, .05, 1e-4, .0);
//   // // pid parameters setting
//   // // pid.init(Kp, Ki, Kd, dt, low_limit, high_limit);
//   // // high_limit's default value = 70.0'C
//   // setPoint = 50.0;

//   // xSemaphore = xSemaphoreCreateMutex();
//   // if (xSemaphore != NULL) {
//   //   Serial.println("Mutex created");
//   // }
//   if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
//     xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
//     if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
//   }

//   xTaskCreate(
//     TaskDhtRead,
//     "DhtRead",
//     512,
//     NULL,
//     0,
//     NULL
//   );

//   xTaskCreate(
//     TaskPidControll,
//     "PidControll",
//     128,
//     NULL,
//     2,
//     NULL
//   );

//   // vTaskStartScheduler();
//   // xCountingSemaphore = xSemaphoreCreateCounting(1,1);
//   // xSemaphoreGive(xCountingSemaphore);
// }

// void loop() {
//     // Data humitmp = dht::check();
//     // tempValue = humitmp.temp;
//     // Serial.println(tempValue);
// }

// void TaskDhtRead(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)5) == pdTRUE) {
//       Serial.println("Inside Task1 and Serial monitor Resource Taken");
//       // Data humitmp = dht::check();
//       // tempValue = humitmp.temp;
//       bool f = dht_sensor.measure(&tempValue, &humiValue);
//       Serial.println(tempValue);
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelay(15);
//   }
// }

// void TaskPidControll(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)5) == pdTRUE) {
//       Serial.println("Inside Task2 and Serial monitor Resource Taken");
//       // Serial.println(temper_pid.pid_control(setPoint, tempValue));
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelay(15);
//   }
// }

// // void loop() {
// //   if(Serial.available()) {
// //     Serial.println("ssibal");
// //     // char c = Serial.read();
// //     color += Serial.parseInt();
// //     Serial.println(color);
// //     return;
// //     delay(10);
// //   }
// //   Serial.flush();
// //   Serial.println(color);
// //   temper_pid.DEBUG();
// //   ld ans = temper_pid.pid_control(50, 30);
// //   Serial.println(ans);

// //   delay(200);
// // }
#include <SoftwareSerial.h>
#include "_config.h"
#include "ledControl.h"
#include "pidControl.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include "DHT_Async.h"

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only one Task is accessing this resource at any time.
#define DHT_SENSOR_TYPE DHT_TYPE_11
SemaphoreHandle_t xSerialSemaphore;

static const int DHT_SENSOR_PIN = 10;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

float temperature;
float humidity;

float setPoint = 50;

PID temper_pid = PID();

// define two Tasks for DigitalRead & AnalogRead
void TaskDigitalRead( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  temper_pid.init(.2, .1, .05, 1e-4, .0);
  // pid parameters setting
  // pid.init(Kp, Ki, Kd, dt, low_limit, high_limit);
  // high_limit's default value = 70.0'C
  setPoint = 50.0;

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  // Now set up two Tasks to run independently.
  xTaskCreate(
    TaskDigitalRead
    ,  "DigitalRead"  // A name just for humans
    ,  400  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL //Parameters for the task
    ,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL ); //Task Handle

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead" // A name just for humans
    ,  400  // Stack size
    ,  NULL //Parameters for the task
    ,  0  // Priority
    ,  NULL ); //Task Handle

  // Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDigitalRead( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    DigitalReadSerial
    Reads a digital input on pin 2, prints the result to the serial monitor

    This example code is in the public domain.
  */

  // digital pin 2 has a pushbutton attached to it. Give it a name:
  uint8_t pushButton = 2;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  xLastWakeTime = xTaskGetTickCount();

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int buttonState = 2;

    TickType_t xS, xE, xExc;
    xS = xTaskGetTickCount();

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the state of the button:
      // Serial.println(buttonState);
      bool f = dht_sensor.measure(&temperature, &humidity);
      // Serial.println(f);
      Serial.println(humidity);
      Serial.flush();
      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    xE = xTaskGetTickCount();
    xExc = xE - xS;
    if (xExc < xFreq) vTaskDelayUntil(&xLastWakeTime, xFreq - xExc);
    else xLastWakeTime = xE;

    // vTaskDelay(15);  // one tick delay (15ms) in between reads for stability
    // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(300));
  }
}

void TaskAnalogRead( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    // read the input on analog pin 0:
    // int sensorValue = analogRead(A0);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the value you read:
      // Serial.println(sensorValue);
      bool f = dht_sensor.measure(&temperature, &humidity);
      // Serial.println(f);
      Serial.println(temperature);
      // Serial.println(temper_pid.pid_control(setPoint, temperature));
      Serial.flush();
      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    // vTaskDelay(pdMS_TO_TICKS(300));  // one tick delay (15ms) in between reads for stability
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
  }
}
