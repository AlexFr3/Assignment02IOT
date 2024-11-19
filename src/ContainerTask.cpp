#include "ContainerTask.h"
#include <Arduino.h>
#include "Sonar.h"
#include "Light.h"
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
void ContainerTask::tick()
{
  switch (this->containerState)
  {
  case NOT_FULL:
    {
      Serial.println("not full");
      float distance = sonar->getDistance();
      Serial.println(distance);
      if (distance < MIN_DISTANCE)
      {
        distance = MIN_DISTANCE;
      }
      else if (distance > MAX_DISTANCE)
      {
        distance = MAX_DISTANCE;
      }
      float percentage = 100 - (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE) * 100;
      Serial.println(percentage);
      /*TODO mandare percentuale da visualizzare*/
      if (distance <= MIN_DISTANCE)
      {
        full = true;
        l1->switchOff();
        l2->switchOn();
        this->containerState = FULL;
      }
      break;
    }
    case FULL:
    {
      Serial.println("full");
      bool pressed= true; /*TODO ricevere messaggio da GUI*/
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
      Serial.println("emptying");
      if (millis() - startEmptying > T3)
      {
        emptying = false;
        this->containerState = NOT_FULL;
        l1->switchOn();
        l2->switchOff();
      }
      break;
    }
  }
}
