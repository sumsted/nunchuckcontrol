#ifndef PIDCONTROLLER
#define PIDCONTROLLER

#include "RoverMotor.h"

class PidController {
    public:
        PidController(RoverMotor roverMotor, double pk, double ik, double dk, long currentTicks);
        runMotor(int currentPidSpeed, long currentTicks);
        setConstants(double pk, double ik, double dk);
    private:
        RoverMotor roverMotor;
        double pk;
        double ik;
        double dk;
        long currentMillis;
        long currentTicks;
        int currentPidSpeed;
        int currentMotorSpeed;
        long lastMillis;
        long lastTicks;
        int lastPidSpeed;
        int lastMotorSpeed;
};

#endif