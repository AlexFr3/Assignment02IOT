#define OPEN_PIN 4
#define CLOSE_PIN 5
#define DOOR_PIN 9
#define PIR_PIN 6
#define TMP_PIN A0
#define TEMPERATURE_PERIOD 100
#define USERDETECTION_PERIOD 100
#define DOOR_PERIOD 150
#define SCHEDULER_PERIOD 150
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
#include "TemperatureTask.h"
#include "output.h"
#include "MsgService.h"
Scheduler sched;
Light *l1;
Light *l2;
bool emptying = false;
bool crytTemp = false;
bool full = false;
bool restorePressed = false;
bool emptyPressed = false;

void setup()
{
  MsgService.init();
  sched.init(SCHEDULER_PERIOD);
  outputInit();
  l1 = new Led(L1_PIN);
  l2 = new Led(L2_PIN);
  l1->switchOn();
  l2->switchOff();
  Task *doorTask = new DoorTask(DOOR_PIN, OPEN_PIN, CLOSE_PIN);
  Task *containerTask = new ContainerTask(l1, l2, ECHO_PIN, TRIG_PIN);
  Task *userDetectionTask = new UserDetectionTask(PIR_PIN);
  Task *temperatureTask = new TemperatureTask(l1, l2, TMP_PIN);
  temperatureTask->init(TEMPERATURE_PERIOD);
  userDetectionTask->init(USERDETECTION_PERIOD);
  doorTask->init(DOOR_PERIOD);
  containerTask->init(CONTAINER_PERIOD);
  sched.addTask(doorTask);
  sched.addTask(containerTask);
  sched.addTask(userDetectionTask);
  sched.addTask(temperatureTask);
  writeMessage("PRESS OPEN ");
  setNextLine();
  writeMessage("TO ENTER WASTE");
}

void loop()
{
  sched.schedule();
}