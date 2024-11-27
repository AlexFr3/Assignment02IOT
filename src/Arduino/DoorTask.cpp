#include "DoorTask.h"
#include <Arduino.h>
#include "output.h"
extern bool emptying;
extern bool crytTemp;
extern bool full;

DoorTask::DoorTask(int doorPin, int openPin, int closePin)
{
  this->door = new ServoMotorImpl(doorPin);
  this->openButton = new ButtonImpl(openPin);
  this->closeButton = new ButtonImpl(closePin);
  this->doorState = CLOSED;
  moveDoor(CLOSED_ANGLE);
}

void DoorTask::tick()
{

  switch (doorState)
  {
  case OPENED:
  {

    long elapsed = millis() - lastOpen;
    bool closePressed = closeButton->isPressed();
    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
    }
    else if (closePressed || elapsed > T1)
    {
      doorState = WASTE_RECEIVED;
      clearOutput();
      writeMessage("WASTE RECEIVED");
      wasteReceivedTime = millis();
      moveDoor(CLOSED_ANGLE);
    }
    break;
  }

  case CLOSED:
  {
    bool openPressed = openButton->isPressed();
    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
    }
    else if (openPressed)
    {
      clearOutput();
      writeMessage("PRESS CLOSE");
      setNextLine();
      writeMessage("WHEN DONE");
      lastOpen = millis();
      doorState = OPENED;
      moveDoor(OPENED_ANGLE);
    }

    break;
  }

  case REVERSED:
  {
    if (!emptying)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
      if (!crytTemp)
      {
        clearOutput();
        writeMessage("PRESS OPEN");
        setNextLine();
        writeMessage("TO ENTER WASTE");
      } else {
        clearOutput();
        writeMessage("PROBLEM DETECTED");
      }
    }
    break;
  }

  case BLOCKED:
  {
    if (emptying)
    {
      doorState = REVERSED;
      moveDoor(REVERSED_ANGLE);
    }
    else if (!full && !crytTemp)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
    }
    break;
  }
  case WASTE_RECEIVED:
  {
    if (millis() - wasteReceivedTime > T2)
    {
      clearOutput();
      writeMessage("PRESS OPEN ");
      setNextLine();
      writeMessage("TO ENTER WASTE");
      doorState = CLOSED;
    }
  }
  }
}

void DoorTask::moveDoor(int angle)
{
  door->on();
  door->setPosition(angle);
  delay(400);
  door->off();
}
