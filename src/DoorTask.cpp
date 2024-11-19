#include "DoorTask.h"
#include <Arduino.h>

extern bool emptying;
extern bool crytTemp;
extern bool full;

DoorTask::DoorTask(Light *l1, Light *l2, int doorPin, int openPin, int closePin)
{
  this->l1 = l1;
  this->l2 = l2;
  this->door = new ServoMotorImpl(doorPin);
  this->openButton = new ButtonImpl(openPin);
  this->closeButton = new ButtonImpl(closePin);

  this->doorState = CLOSED;
  l1->switchOn();
  l2->switchOff();
  Serial.println("Inizializzazione fatta");
  /*door->on();
  door->setPosition(90);
  delay(100);
  door->off();*/
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
    if (closePressed || elapsed > T1)
    {
      doorState = CLOSED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("chiudi porta");
    }

    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta bloccata");
    }
    break;
  }

  case CLOSED:
  {
    Serial.println("Check del open");
    bool openPressed = openButton->isPressed();
    if (openPressed)
    {
      lastOpen = millis();
      doorState = OPENED;
      moveDoor(OPENED_ANGLE);
      Serial.println("apri porta");
    }

    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta bloccata");
    }

    if (emptying)
    {
      doorState = REVERSED;
      moveDoor(REVERSED_ANGLE);
      Serial.println("porta invertita");
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
    if (!full && !crytTemp)
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
  /*door->on();
  door->setPosition(angle);
  delay(1000);
  door->off();*/
}
