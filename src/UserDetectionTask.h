#ifndef __USERDETECTIONTASK__
#define __USERDETECTIONTASK__
#define TSLEEP 10000
#include "Task.h"
#include <Arduino.h>
#include "Pir.h"

class UserDetectionTask : public Task
{
    UserDetector *pir;
    long lastSeen;
    enum
    {
        AWAKE,
        SLEEP
    } userDetectionState;

public:
    UserDetectionTask(int pirPin);
    void tick();
};
#endif