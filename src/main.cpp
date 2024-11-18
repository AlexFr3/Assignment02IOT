#include <Arduino.h>
#include "Wire.h"
#include "Scheduler.h"
#include "Led.h"
#include "DoorTask.h"
Scheduler sched;
Light* l1;
Light* l2;


void setup() {
  Serial.begin(9600);
  sched.init(150);
  l1 = new Led(2);
  l2 = new Led(3);
  //open sul pin 4, close sul 5
  Task* doorTask = new DoorTask(l1, l2, 9, 4, 5);
  doorTask->init(150);
  sched.addTask(doorTask);
}

void loop() {
  sched.schedule();
}