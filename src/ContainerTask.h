#ifndef __CONTAINERTASK__
#define __CONTAINERTASK__
#include "Task.h"
#include "Light.h"
#include "DistanceDevice.h"
#include <Arduino.h>

class ContainerTask : public Task
{
  Light* l1;
  Light* l2;
  DistanceDevice* sonar;
  enum {FULL, NOT_FULL,EMPTYING} containerState;
public:
  ContainerTask(Light* l1, Light* l2,int echoPin, int trigPin);
  void tick();
};
#endif