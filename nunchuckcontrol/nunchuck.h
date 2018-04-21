#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#include <Arduino.h>
#include <Wire.h>

typedef struct {
    uint8_t xPosition;
    uint8_t yPosition;
    uint8_t xAccelRaw;
    uint8_t yAccelRaw;
    uint8_t zAccelRaw;
    uint8_t buttonsRaw;
} NunchuckRawDataType;

union NunchuckRawDataUnion{
    NunchuckRawDataType data;
    unsigned char buffer[6];
};

typedef struct {
    uint8_t xPosition;
    uint8_t yPosition;
    uint8_t zButton;
    uint8_t cButton;
} NunchuckJoystickType;

typedef struct {
    uint8_t xAngle;
    uint8_t yAngle;
    uint8_t zAngle;
} NunchuckAccelType;

class Nunchuck {
    public:
        Nunchuck();
        NunchuckJoystickType *getJoystick();
        NunchuckAccelType *getAccel();
        int read();
        void print();
        void chart();
    private:
        NunchuckRawDataUnion raw;
        NunchuckJoystickType joystick;
        NunchuckAccelType accel;
        char decodeByte(char x);
        void sendCommand();
        void transform();
};

#endif