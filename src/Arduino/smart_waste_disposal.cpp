#include "initialization.h"


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