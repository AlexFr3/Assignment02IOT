#ifndef __SONAR__
#define __SONAR__
#include "DistanceDevice.h"

class Sonar : public DistanceDevice
{

public:
    Sonar(int trigPin, int echoPin);
    float getDistance();

private:
    int trigPin;
    int echoPin;
};

#endif