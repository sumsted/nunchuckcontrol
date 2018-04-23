#ifndef ROVERMOTOR_H
#define ROVERMOTOR_H

#include <Arduino.h>
#include <Servo.h>

#define PWM_PIN_LF 5
#define DIR_PIN_LF 10
#define PWM_PIN_RF 9
#define DIR_PIN_RF 6
#define PWM_LEFT_CHANNEL 0
#define PWM_RIGHT_CHANNEL 1
#define PWM_FREQUENCY 5000
#define PWM_TIMER_13_BIT 13
#define MOTOR_ORIENTATION_LEFT -1
#define MOTOR_ORIENTATION_RIGHT 1
#define PWM_FULL_FORWARD 0
#define PWM_STOP 127
#define PWM_FULL_BACKWARD 255
#define PWM_DELTA 127
#define PWM_TUNE_PERCENTAGE .75
#define MOTOR_SMOOTHING_ITERATIONS 10
#define MOTOR_SMOOTHING_WAIT 35 // ms

class RoverMotor(){
    public:
        RoverMotor();
        void testPwm();
        int runMotor(int leftSpeed, int rightSpeed);
    private:
        int lastLeftPulse;
        int lastRightPulse;
}

##endif