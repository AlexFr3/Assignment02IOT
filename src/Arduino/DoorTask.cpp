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
    bool closePressed = closeButton->isPressed();
    if (full || crytTemp)
    {
      doorState = BLOCKED;
      moveDoor(CLOSED_ANGLE);
      Serial.println("porta bloccata");
    }
    else if (closePressed || elapsed > T1)
    {
      doorState = WASTE_RECEIVED;
      clearOutput();
      writeMessage("WASTE RECEIVED");
      wasteReceivedTime = millis();
      moveDoor(CLOSED_ANGLE);
      Serial.println("chiudi porta");
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
      Serial.println("porta bloccata");
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
      clearOutput();
      writeMessage("PRESS OPEN");
      setNextLine();
      writeMessage("TO ENTER WASTE");
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
