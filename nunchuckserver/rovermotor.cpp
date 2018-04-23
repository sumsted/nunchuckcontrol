#include <Arduino.h>
#include <Servo.h>


RoverMotor::RoverMotor(){
    lastLeftPulse = PWM_STOP;
    lastRightPulse = PWM_STOP;
    pinMode(DIR_PIN_LF, OUTPUT);
    pinMode(PWM_PIN_LF, OUTPUT);
    pinMode(DIR_PIN_RF, OUTPUT);
    pinMode(PWM_PIN_RF, OUTPUT);
    pinMode(US_FRONT_TRIG_PIN, OUTPUT);
    pinMode(US_FRONT_ECHO_PIN, INPUT);

    digitalWrite(PWM_PIN_LF, HIGH);
    digitalWrite(PWM_PIN_RF, HIGH);
    analogSetup(DIR_PIN_LF, PWM_LEFT_CHANNEL);
    analogSetup(DIR_PIN_RF, PWM_RIGHT_CHANNEL);
    analogWrite(DIR_PIN_LF, PWM_STOP);
    analogWrite(DIR_PIN_RF, PWM_STOP);
}

void RoverMotor::analogSetup(int pin, int channel){
    ledcSetup(channel, PWM_FREQUENCT, PWM_TIMER_13_BIT);
    ledcAttachPin(pin, channel);
    analogWrite(channel, PWM_STOP);
}

void RoverMotor::analogWrite(int channel, int pulse){
    ledcWrite(channel, pulse);
}

int RoverMotor::runMotor(int leftSpeed, int rightSpeed1){
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
        analogWrite(DIR_PIN_LF, lastLeftPulse);
        analogWrite(DIR_PIN_RF, lastRightPulse);
    } else {
        leftPulse = PWM_STOP + (PWM_DELTA * (leftSpeed * PWM_TUNE_PERCENTAGE)/100 * MOTOR_ORIENTATION_LEFT);
        rightPulse = PWM_STOP + (PWM_DELTA * (rightSpeed * PWM_TUNE_PERCENTAGE)/100 * MOTOR_ORIENTATION_RIGHT);
        deltaLeftPulse = (leftPulse - lastLeftPulse) / MOTOR_SMOOTHING_ITERATIONS;
        deltaRightPulse = (rightPulse - lastRightPulse) / MOTOR_SMOOTHING_ITERATIONS;
        for(i=0;i<MOTOR_SMOOTHING_ITERATIONS;i++){
            unsigned long l = lastLeftPulse+(deltaLeftPulse*i);
            unsigned long r = lastRightPulse+(deltaRightPulse*i);
            analogWrite(DIR_PIN_LF, l);
            analogWrite(DIR_PIN_RF, r);
//            Serial.print(l);
//            Serial.print(", ");
//            Serial.println(r);
            delay(MOTOR_SMOOTHING_WAIT);
        }
        analogWrite(DIR_PIN_LF, leftPulse);
        analogWrite(DIR_PIN_RF, rightPulse);
        commandProcessed = true;
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
        analogWrite(DIR_PIN_LF, pulse);
        analogWrite(DIR_PIN_RF, pulse);
        delay(50);
        Serial.println(pulse);        
    }    
    for(pulse=255;pulse>=0;pulse--){
        analogWrite(DIR_PIN_LF, pulse);
        analogWrite(DIR_PIN_RF, pulse);
        delay(50);
        Serial.println(pulse);        
    }    
}
