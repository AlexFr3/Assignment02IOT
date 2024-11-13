#ifndef __DOORTASK__
#define __DOORTASK__

#include "Task.h"
#include "Light.h"
class DoorTask : public Task
{
private:
    int pin;
    Light* l1;
    Light* l2;
    enum {OPENED, CLOSED,REVERSED} doorState;
public:
    DoorTask(/* args */);
    ~DoorTask();
};
#endif