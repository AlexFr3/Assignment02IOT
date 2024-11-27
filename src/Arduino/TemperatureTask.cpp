#include "TemperatureTask.h"
#include <Arduino.h>
#include "Light.h"
#include "output.h"
#include "TemperatureDevice.h"
#include "MsgService.h"

extern bool crytTemp;
extern bool full;
extern bool emptyPressed;
extern bool restorePressed;

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
    MsgService.sendMsg("Temperature: " + String(temperature) + "°");
    switch (this->temperatureState)
    {
    case STABLE:
    {
        if (restorePressed)
        {
            restorePressed = false;
        }
        if (MsgService.isMsgAvailable())
        {
            Msg *msg = MsgService.receiveMsg();

            if (msg->getContent() == "EMPTY_CONTAINER")
            {
                emptyPressed = true;
            }
            delete msg;
        }
        if (temperature <= MAXTEMP)
        {
            lastStable = millis();
        }
        else
        {
            if (millis() - lastStable > MAXTEMPTIME)
            {
                clearOutput();
                writeMessage("PROBLEM DETECTED");
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
        if (restorePressed)
        {
            crytTemp = false;
            this->temperatureState = STABLE;
            restorePressed = false;
            clearOutput();
            if (!full)
            {
                writeMessage("PRESS OPEN ");
                setNextLine();
                writeMessage("TO ENTER WASTE");
                l1->switchOn();
                l2->switchOff();
            }
            else
            {
                writeMessage("CONTAINER FULL");
            }
        }
        break;
    }
    }
}