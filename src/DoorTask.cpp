#include "DoorTask.h"
#include <Arduino.h>

DoorTask::DoorTask(Light* l1, Light* l2, int doorPin, int openPin, int closePin)
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
  delay(100);*/
}

void DoorTask::tick()
{
  Serial.println("stato: " + String(this->doorState));
        switch (doorState) {
        case OPENED: {
          Serial.println("Check del closed");
          bool closePressed = closeButton->isPressed();
          if(closePressed || millis() - lastOpen > 5000){
            doorState = CLOSED;
            /*door->on();
            door->setPosition(90);
            delay(1000);
            door->off();*/
            Serial.println("chiudi porta");
          }
          break;
        }
          
        case CLOSED: {
          Serial.println("Check del open");
          bool openPressed = openButton->isPressed();
          if(openPressed){
              lastOpen=millis();
              doorState = OPENED;
              /*door->on();
              door->setPosition(180);
              door->off();*/
              Serial.println("apri porta");
          }            
          break;
        }
            
        case REVERSED: {
          Serial.println("Caso REVERSED");
          break;
        }
    }
    Serial.println("dopo switch: " + String(doorState));
    }
