#include "rovermotor.h"

RoverMotor::RoverMotor(){
    lastLeftPulse = PWM_STOP;
    lastRightPulse = PWM_STOP;
    pinMode(DIR_PIN_LF, OUTPUT);
    pinMode(PWM_PIN_LF, OUTPUT);
    pinMode(DIR_PIN_RF, OUTPUT);
    pinMode(PWM_PIN_RF, OUTPUT);

    digitalWrite(PWM_PIN_LF, HIGH);
    digitalWrite(PWM_PIN_RF, HIGH);
    analogSetup(DIR_PIN_LF, PWM_LEFT_CHANNEL);
    analogSetup(DIR_PIN_RF, PWM_RIGHT_CHANNEL);
    analogWrite(PWM_LEFT_CHANNEL, PWM_STOP);
    analogWrite(PWM_RIGHT_CHANNEL, PWM_STOP);
}

void RoverMotor::analogSetup(int pin, int channel){
    ledcSetup(channel, PWM_FREQUENCY, PWM_TIMER_BITS);
    ledcAttachPin(pin, channel);
    analogWrite(channel, PWM_STOP);
}

void RoverMotor::analogWrite(int channel, int pulse){
    ledcWrite(channel, pulse);
}

int RoverMotor::runMotor(int leftSpeed, int rightSpeed){
    int leftPulse = PWM_STOP;
    int rightPulse = PWM_STOP;
    int i;
    int deltaLeftPulse;
    int deltaRightPulse;

    leftPulse = PWM_STOP;
    rightPulse = PWM_STOP;

    digitalWrite(PWM_PIN_LF, HIGH);
    digitalWrite(PWM_PIN_RF, HIGH);

    if(leftSpeed > 100 || leftSpeed < -100 || rightSpeed > 100 || rightSpeed < -100){
        analogWrite(PWM_LEFT_CHANNEL, lastLeftPulse);
        analogWrite(PWM_RIGHT_CHANNEL, lastRightPulse);
    } else {
        leftPulse = PWM_STOP + (PWM_DELTA * (leftSpeed * PWM_TUNE_PERCENTAGE)/100 * MOTOR_ORIENTATION_LEFT);
        rightPulse = PWM_STOP + (PWM_DELTA * (rightSpeed * PWM_TUNE_PERCENTAGE)/100 * MOTOR_ORIENTATION_RIGHT);
        deltaLeftPulse = (leftPulse - lastLeftPulse) / MOTOR_SMOOTHING_ITERATIONS;
        deltaRightPulse = (rightPulse - lastRightPulse) / MOTOR_SMOOTHING_ITERATIONS;
        for(i=0;i<MOTOR_SMOOTHING_ITERATIONS;i++){
            unsigned long l = lastLeftPulse+(deltaLeftPulse*i);
            unsigned long r = lastRightPulse+(deltaRightPulse*i);
            analogWrite(PWM_LEFT_CHANNEL, l);
            analogWrite(PWM_RIGHT_CHANNEL, r);
            Serial.print(leftSpeed*10);Serial.print("\t");
            Serial.print(rightSpeed*10);Serial.print("\t");
            Serial.print(l);Serial.print("\t");
            Serial.println(r);
            delay(MOTOR_SMOOTHING_WAIT);
        }
        analogWrite(PWM_LEFT_CHANNEL, leftPulse);
        analogWrite(PWM_RIGHT_CHANNEL, rightPulse);
        Serial.print(leftSpeed*10);Serial.print("\t");
        Serial.print(rightSpeed*10);Serial.print("\t");
        Serial.print(leftPulse);Serial.print("\t");
        Serial.println(rightPulse);
    }

    lastLeftPulse = leftPulse;
    lastRightPulse = rightPulse;
    return 0;
}

void RoverMotor::testPwm(){
    int pwm_value = 0;
    int opposite_i;
    int pulse;
    digitalWrite(PWM_PIN_LF, HIGH);
    digitalWrite(PWM_PIN_RF, HIGH);
    for(pulse=0;pulse<=255;pulse++){
        analogWrite(PWM_LEFT_CHANNEL, pulse);
        analogWrite(PWM_RIGHT_CHANNEL, pulse);
        delay(50);
        Serial.println(pulse);
    }
    for(pulse=255;pulse>=0;pulse--){
        analogWrite(PWM_LEFT_CHANNEL, pulse);
        analogWrite(PWM_RIGHT_CHANNEL, pulse);
        delay(50);
        Serial.println(pulse);
    }
}

void RoverMotor::testLR(){
    int i = 0;
    for(i = -100;i < 101;i++){
        runMotor(i, i);
    }
    for(i = 100;i > -101;i--){
        runMotor(i, i);
    }
}

void RoverMotor::testZero(){
    int i = 0;
    runMotor(i, i);
}
