#include "TemperatureTask.h"
#include <Arduino.h>
#include "Light.h"
#include "output.h"
#include "TemperatureDevice.h"
extern bool crytTemp;
extern bool full;
TemperatureTask::TemperatureTask(Light *l1, Light *l2, int temperaturePin)
{
    this->l1 = l1;
    this->l2 = l2;
    this->tmp = new TemperatureSensor(temperaturePin);
    this->temperatureState = STABLE;
    this->lastStable = millis();
}
void TemperatureTask::tick()
{
    float temperature = tmp->getTemperature();
    Serial.println("Temperatura: " + String(temperature));
    switch (this->temperatureState)
    {
    case STABLE:
    {
        if (temperature <= MAXTEMP)
        {
            Serial.println("temperatura stabile");
            lastStable = millis();
        }
        else
        {
            if (millis() - lastStable > MAXTEMPTIME)
            {
                clearOutput();
                writeMessage("PROBLEM DETECTED");
                Serial.println("critical temperature");
                this->temperatureState = CRITICAL;
                crytTemp = true;
                l1->switchOff();
                l2->switchOn();
            }
        }
        break;
    }

    case CRITICAL:
    {
        bool pressed = true; /*TODO*/
        Serial.println("critical temperature");
        if (pressed)
        {
            crytTemp = false;
            this->temperatureState = STABLE;
            clearOutput();
            if (!full)
            {
                writeMessage("PRESS OPEN ");
                setNextLine();
                writeMessage("TO ENTER WASTE");
                l1->switchOn();
                l2->switchOff();
            }else{
                writeMessage("CONTAINER FULL");
            }
            
        }
        break;
    }
    }
}