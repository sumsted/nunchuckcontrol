#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#include <Wire.h>

typedef struct {
    uint8_t xPosition;
    uint8_t yPosition;
    uint8_t xAccelRaw;
    uint8_t yAccelRaw;
    uint8_t zAccelRaw;
    uint8_t buttonsRaw;
} NunchuckDataType;

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
        void read();
        void printData();
    private:
        NunchuckDataType data;
        NunchuckJoystickType joystick;
        NunchuckAccelType accel;
        void sendCommand();
        void transform();
}

#endif NUNCHUCK_H