#include "Sonar.h"
#include "Arduino.h"


const float temperature = 20;
const float vs = 331.45 + 0.62*temperature;

Sonar::Sonar(int trigPin, int echoPin){
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}

int Sonar::getDistance(){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(3);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin,LOW);
    
    /* Receiving the echo */

    float tUS = pulseIn(echoPin, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t*vs;
    return d;
};