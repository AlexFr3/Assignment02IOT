#include "DoorTask.h"
#include <Arduino.h>

extern bool emptying;
extern bool crytTemp;
extern bool full;

DoorTask::DoorTask(int doorPin, int openPin, int closePin)
{
  this->door = new ServoMotorImpl(doorPin);
  this->openButton = new ButtonImpl(openPin);
  this->closeButton = new ButtonImpl(closePin);
  this->doorState = CLOSED;
  Serial.println("Inizializzazione fatta");
  moveDoor(CLOSED_ANGLE);
}

void DoorTask::tick()
{

  Serial.println("stato: " + String(this->doorState));
  switch (doorState)
  {
  case OPENED:
  {

    long elapsed = millis() - lastOpen;
    Serial.println("elapsed: " + String(elapsed));
    Serial.println("Check del closed");
    bool closePressed = closeButton->isPressed();
    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta bloccata");
    }
    else if (closePressed || elapsed > T1)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("chiudi porta");
    }
    break;
  }

  case CLOSED:
  {
    Serial.println("Check del open");
    bool openPressed = openButton->isPressed();
    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta bloccata");
    }
    else if (openPressed)
    {
      lastOpen = millis();
      doorState = OPENED;
      moveDoor(OPENED_ANGLE);
      Serial.println("apri porta");
    }

    break;
  }

  case REVERSED:
  {
    Serial.println("Caso REVERSED");
    if (!emptying)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta chiusa");
    }
    break;
  }

  case BLOCKED:
  {
    Serial.println("Caso BLOCKED");
    if (emptying)
    {
      doorState = REVERSED;
      moveDoor(REVERSED_ANGLE);
      Serial.println("porta invertita");
    }
    else if (!full && !crytTemp)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta chiusa");
    }
    break;
  }
    Serial.println("dopo switch: " + String(doorState));
  }
}

void DoorTask::moveDoor(int angle)
{
  door->on();
  door->setPosition(angle);
  delay(1000);
  door->off();
}
