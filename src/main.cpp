#define  OPEN_PIN 4
#define  CLOSE_PIN 5
#define  DOOR_PIN 9
#define DOOR_PERIOD 150
#define SCHEDULER_PERIOD 150
#define L1_PIN 2
#define L2_PIN 3
#include <Arduino.h>
#include "Wire.h"
#include "Scheduler.h"
#include "Led.h"
#include "DoorTask.h"
Scheduler sched;
Light* l1;
Light* l2;
bool emptying = false;
bool crytTemp = false;
bool full = false;

void setup() {
  Serial.begin(9600);
  sched.init(SCHEDULER_PERIOD);
  l1 = new Led(L1_PIN);
  l2 = new Led(L2_PIN);
  //open sul pin 4, close sul 5
  Task* doorTask = new DoorTask(l1, l2, DOOR_PIN, OPEN_PIN, CLOSE_PIN);
  doorTask->init(DOOR_PERIOD);
  sched.addTask(doorTask);
}

void loop() {
  sched.schedule();
}