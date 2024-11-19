#include "ContainerTask.h"
#include <Arduino.h>
#include "Sonar.h"

extern bool emptying;
extern bool crytTemp;
extern bool full;

ContainerTask::ContainerTask(Light *l1, Light *l2, int echoPin, int trigPin)
{
  this->l1 = l1;
  this->l2 = l2;
  this->sonar = new Sonar(trigPin, echoPin);
  this->containerState = NOT_FULL;
}
