#ifndef __DOORTASK__
#define __DOORTASK__

#define T1 5000
#define CLOSED_ANGLE 90
#define OPENED_ANGLE 180
#define REVERSED_ANGLE 0

#include "Task.h"
#include "ButtonImpl.h"
#include <Arduino.h>
#include "servo_motor_impl.h"

class DoorTask : public Task
{
  long lastOpen;
  enum {OPENED, CLOSED,REVERSED, BLOCKED} doorState;
  Button* openButton;
  Button* closeButton;
  ServoMotor* door;
public:
  DoorTask(int doorPin, int openPin, int closePin);
  void tick();
  void moveDoor(int angle);
};
#endif