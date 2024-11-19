#ifndef __CONTAINERTASK__
#define __CONTAINERTASK__
#define MAX_DISTANCE 1
#define MIN_DISTANCE 0.1
#define T3 5000
#include "Task.h"
#include "Light.h"
#include "DistanceDevice.h"
#include <Arduino.h>

class ContainerTask : public Task
{
  Light* l1;
  Light* l2;
  DistanceDevice* sonar;
  long startEmptying;
  enum {FULL, NOT_FULL,EMPTYING} containerState;
public:
  ContainerTask(Light* l1, Light* l2,int echoPin, int trigPin);
  void tick();
};
#endif