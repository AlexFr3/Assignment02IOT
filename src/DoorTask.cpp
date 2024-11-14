#include "DoorTask.h"
#include <Arduino.h>

DoorTask::DoorTask(Light* l1, Light* l2, int doorPin, int openPin, int closePin)
{
  this->l1 = l1;
        this->l2 = l2;
        this->doorPin = doorPin;
        this->openButton = new ButtonImpl(openPin);
        this->closeButton = new ButtonImpl(closePin);
        doorState = CLOSED;
}
void DoorTask::tick()
{
        switch(doorState){
            case OPENED:
                l1->switchOn();
                l2->switchOff();
                if(closeButton->isPressed()){
                    doorState = REVERSED;
                }
                break;
            case CLOSED:
                l1->switchOff();
                l2->switchOn();
                if(openButton->isPressed()){
                    doorState = REVERSED;
                }
                break;
            case REVERSED:
                l1->switchOn();
                l2->switchOn();
                if(digitalRead(doorPin) == HIGH){
                    doorState = OPENED;
                } else {
                    doorState = CLOSED;
                }
                break;
        }
    }
