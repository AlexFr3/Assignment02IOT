#define  OPEN_PIN 4
#define  CLOSE_PIN 5
#define  DOOR_PIN 9
#define PIR_PIN 6
#define USERDETECTION_PERIOD 100
#define DOOR_PERIOD 150
#define SCHEDULER_PERIOD 1000
#define L1_PIN 2
#define L2_PIN 3
#define TRIG_PIN 13
#define ECHO_PIN 12
#define CONTAINER_PERIOD 50
#include <Arduino.h>
#include "Wire.h"
#include "Scheduler.h"
#include "Led.h"
#include "DoorTask.h"
#include "ContainerTask.h"
#include "UserDetectionTask.h"
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
  l1->switchOn();
  l2->switchOff();
  //open sul pin 4, close sul 5
  Task* doorTask = new DoorTask(DOOR_PIN, OPEN_PIN, CLOSE_PIN);
  //trig sul pin 13, echo sul 12
  Task* containerTask= new ContainerTask(l1, l2, ECHO_PIN, TRIG_PIN); 
  Task* userDetectionTask = new UserDetectionTask(PIR_PIN);
  userDetectionTask->init(USERDETECTION_PERIOD);
  doorTask->init(DOOR_PERIOD);
  containerTask->init(CONTAINER_PERIOD);
  sched.addTask(doorTask);
  sched.addTask(containerTask);
  sched.addTask(userDetectionTask);
}

void loop() {
  sched.schedule();
}