#include "ContainerTask.h"
#include <Arduino.h>
#include "Sonar.h"
#include "Light.h"
#include "output.h"
#include "MsgService.h"
extern bool emptying;
extern bool crytTemp;
extern bool full;

ContainerTask::ContainerTask(Light *l1, Light *l2, int echoPin, int trigPin)
{
  this->l1 = l1;
  this->l2 = l2;
  this->sonar = new Sonar(trigPin, echoPin);
  this->containerState = NOT_FULL;
  sonar->getDistance();
}
void ContainerTask::tick()
{
  switch (this->containerState)
  {
  case NOT_FULL:
  {
    float distance = sonar->getDistance();
    if (distance < MIN_DISTANCE)
    {
      distance = MIN_DISTANCE;
    }
    else if (distance > MAX_DISTANCE)
    {
      distance = MAX_DISTANCE;
    }
    float percentage = 100 - (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE) * 100;
    MsgService.sendMsg("Percentuale di riempimento: " + String(percentage) + "%");
    if (distance <= MIN_DISTANCE)
    {
      clearOutput();
      writeMessage("CONTAINER FULL");
      full = true;
      l1->switchOff();
      l2->switchOn();
      this->containerState = FULL;
    }
    break;
  }
  case FULL:
  {
    bool pressed = true; /*TODO ricevere messaggio da GUI*/
    if (pressed)
    {
      emptying = true;
      this->containerState = EMPTYING;
      startEmptying = millis();
    }
    break;
  }
  case EMPTYING:
  {
    if (millis() - startEmptying > T3)
    {
      emptying = false;
      full = false;
      this->containerState = NOT_FULL;
      if (!crytTemp)
      {
        l1->switchOn();
        l2->switchOff();
      }
    }
    break;
  }
  }
}
