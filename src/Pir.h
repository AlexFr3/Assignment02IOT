#ifndef __PIR__
#define __PIR__
#include "UserDetector.h"

class Pir : public UserDetector
{

public:
    Pir(int pin);
    bool isDetected();

private:
    int pin;
};

#endif