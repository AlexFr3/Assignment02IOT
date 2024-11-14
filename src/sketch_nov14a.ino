#include <Arduino.h>
#include <Wire.h>
#include "Scheduler.h"
Scheduler sched;
void setup() {
  sched.init(150);
}

void loop() {
  sched.schedule();
}

