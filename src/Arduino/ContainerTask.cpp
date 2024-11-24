#include "ContainerTask.h"
#include <Arduino.h>
#include "Sonar.h"
#include "Light.h"
#include "output.h"
#include "MsgService.h"
extern bool emptying;
extern bool crytTemp;
extern bool full;
extern bool emptyPressed;
extern bool restorePressed;

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
    if (emptyPressed)
    {
      emptyPressed = false;
    }
    if (MsgService.isMsgAvailable())
    {
      Msg *msg = MsgService.receiveMsg();

      if (msg->getContent() == "RESTORE_PRESS")
      {
        restorePressed = true;
      }
      delete msg;
    }
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
    MsgService.sendMsg("Filling percentage: " + String(percentage) + "%");
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
    
    MsgService.sendMsg("Filling percentage: 100%");
    if (MsgService.isMsgAvailable())
    {
      Msg *msg = MsgService.receiveMsg();
      if (msg->getContent() == "EMPTY_CONTAINER")
      {
        emptyPressed = true;
      }
      else if (msg->getContent() == "RESTORE_PRESS")
      {
        restorePressed = true;
      }
      delete msg;
    }

    if (emptyPressed)
    {
      emptying = true;
      this->containerState = EMPTYING;
      startEmptying = millis();
      emptyPressed = false;
    }
    break;
  }

  case EMPTYING:
  {
    if (emptyPressed)
    {
      emptyPressed = false;
    }
    if (MsgService.isMsgAvailable())
    {
      Msg *msg = MsgService.receiveMsg();
      if (msg->getContent() == "RESTORE_PRESS")
      {
        restorePressed = true;
      }
      delete msg;
    }
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