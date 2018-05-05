#ifndef ROVERMOTOR_H
#define ROVERMOTOR_H

#include <Arduino.h>
//#include <Servo.h>

//#define PWM_PIN_LF 18
//#define DIR_PIN_LF 23
//#define PWM_PIN_RF 19
//#define DIR_PIN_RF 22
#define PWM_PIN_LF 2
#define DIR_PIN_LF 17
#define PWM_PIN_RF 23
#define DIR_PIN_RF 19
#define PWM_LEFT_CHANNEL 0
#define PWM_RIGHT_CHANNEL 1
#define PWM_FREQUENCY 5000
#define PWM_TIMER_BITS 8
#define MOTOR_ORIENTATION_LEFT -1
#define MOTOR_ORIENTATION_RIGHT 1
#define PWM_FULL_FORWARD 0
#define PWM_STOP 127
#define PWM_FULL_BACKWARD 255
#define PWM_DELTA 127
#define PWM_TUNE_PERCENTAGE 1.0
#define MOTOR_SMOOTHING_ITERATIONS 4
#define MOTOR_SMOOTHING_WAIT 10 // ms

class RoverMotor{
    public:
        RoverMotor();
        void testPwm();
        void testLR();
        void testZero();
        int runMotor(int leftSpeed, int rightSpeed);
    private:
        int lastLeftPulse;
        int lastRightPulse;
        void analogSetup(int pin, int channel);
        void analogWrite(int channel, int pulse);
};

#endif