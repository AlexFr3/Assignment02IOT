#ifndef __DOORTASK__
#define __DOORTASK__

#include "Task.h"
#include "Light.h"
#include "ButtonImpl.h"
#include <Arduino.h>
class DoorTask : public Task
{
  int doorPin;
  Light* l1;
  Light* l2;
  enum {OPENED, CLOSED,REVERSED} doorState;
  Button* openButton;
  Button* closeButton;
public:
  DoorTask(Light* l1, Light* l2, int doorPin, int openPin, int closePin);
  void tick();
};
#endif