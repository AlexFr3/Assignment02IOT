#ifndef __INITIALIZATION__
#define __INITIALIZATION__

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

#endif
