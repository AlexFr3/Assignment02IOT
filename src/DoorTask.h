#ifndef __DOORTASK__
#define __DOORTASK__

#include "Task.h"
#include "Light.h"
#include "ButtonImpl.h"
#include <Arduino.h>
#include "servo_motor_impl.h"

class DoorTask : public Task
{
  int lastOpen;
  Light* l1;
  Light* l2;
  enum {OPENED, CLOSED,REVERSED} doorState;
  Button* openButton;
  Button* closeButton;
  ServoMotor* door;
public:
  DoorTask(Light* l1, Light* l2, int doorPin, int openPin, int closePin);
  void tick();
};
#endif