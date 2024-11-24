#ifndef __TEMPERATURETASK__
#define __TEMPERATURETASK__
#define MAXTEMP 35
#define MAXTEMPTIME 1000
#include "Task.h"
#include "Light.h"
#include "TemperatureSensor.h"
#include "MsgService.h"
#include <Arduino.h>

class TemperatureTask : public Task
{
    Light *l1;
    Light *l2;
    TemperatureDevice* tmp;
    long lastStable;
    enum{STABLE, CRITICAL} temperatureState;

public:
    TemperatureTask(Light *l1, Light *l2, int temperaturePin);
    void tick();
};
#endif