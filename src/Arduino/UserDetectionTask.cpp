#include "UserDetectionTask.h"
#include <Arduino.h>
#include <avr/sleep.h>
#include <EnableInterrupt.h>
void wakeUp(){}

UserDetectionTask::UserDetectionTask(int pirPin)
{
    this->pir = new Pir(pirPin);
    this->userDetectionState = AWAKE;
    this->lastSeen = millis();
    enableInterrupt(pirPin, wakeUp, RISING);
}
void UserDetectionTask::tick()
{
    if (pir->isDetected())
    {
        lastSeen = millis();
    }
    else
    {
        long elapsed = millis() - lastSeen;
        if (elapsed > TSLEEP)
        {
            Serial.println("Sleeping");
            Serial.flush();
            userDetectionState = SLEEP;
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_mode();
            sleep_disable();
            userDetectionState = AWAKE;
            Serial.println("Awake");
        }
    }
}
