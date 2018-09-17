#include "rovermotor.h"


RoverMotor *roverMotor;

unsigned long lastCheck;
long left = 0;
long right = 0;

void setup(){
    Serial.begin(115200);
    roverMotor = new RoverMotor();
    roverMotor->runMotor(left, right);
}

void loop(){
    if(false){
        Serial.print(left); Serial.print("\t");
        Serial.println(right);
        lastCheck = millis();
    }

    if((millis()-lastCheck)>1000){
        Serial.println("lastTime reset");
        left = 0;
        right = 0;
    }

    Serial.print(left); Serial.print("\t");
    Serial.println(right);
    roverMotor->runMotor(left, right);
}
