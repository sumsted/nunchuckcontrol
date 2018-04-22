#include "nunchuck.h"

Nunchuck *nunchuck;

uint8_t xRange[3] = {35, 130, 225};
uint8_t yRange[3] = {26, 124, 220};

#define HIGH_JOYSTICK_RANGE  2
#define CENTER_JOYSTICK_RANGE  1
#define LOW_JOYSTICK_RANGE  0
long left = 0;
long right = 0;


void setup()
{
    Serial.begin(19200);
    nunchuck = new Nunchuck();
    centerJoystick();
}

void loop()
{
    nunchuck->read();
    transform();
    Serial.print(left); Serial.print("\t");
    Serial.print(right); Serial.print("\n");
    delay(20);
}

void centerJoystick(){
    delay(100);
    nunchuck->read();
    xRange[CENTER_JOYSTICK_RANGE] = nunchuck->joystick.xPosition;
    yRange[CENTER_JOYSTICK_RANGE] = nunchuck->joystick.yPosition;
    xRange[LOW_JOYSTICK_RANGE] = xRange[CENTER_JOYSTICK_RANGE] - 95;
    yRange[LOW_JOYSTICK_RANGE] = yRange[CENTER_JOYSTICK_RANGE] - 95;
    xRange[HIGH_JOYSTICK_RANGE] = xRange[CENTER_JOYSTICK_RANGE] + 95;
    yRange[HIGH_JOYSTICK_RANGE] = yRange[CENTER_JOYSTICK_RANGE] + 95;
}

void transform(){
    long x = (long) ((double)(nunchuck->joystick.xPosition - xRange[LOW_JOYSTICK_RANGE]) / (double)(xRange[HIGH_JOYSTICK_RANGE] - xRange[LOW_JOYSTICK_RANGE]) * 100);
    long y = (long)((double)(nunchuck->joystick.yPosition - yRange[LOW_JOYSTICK_RANGE]) / (double)(yRange[HIGH_JOYSTICK_RANGE] - yRange[LOW_JOYSTICK_RANGE]) * 100);
    x = (x * 2) - 100;
    y = (y * 2) - 100;
    x = long((double)x / 20.0) * 20.0;
    y = long((double)y / 20.0) * 20.0;
    left = (x < 0) ? long((double)y + ((double)x / 100.0 * (double)y)) : y;
    right = (x > 0) ? long((double)y - ((double)x / 100.0 * (double)y)) : y;
    left = (y == 0) ? x * .5 : left;
    right = (y == 0) ? -x * .5 : right;
    left = (abs(left) > 100) ? ((left/abs(left))*100) : left;
    right= (abs(right) > 100) ? ((right/abs(right))*100) : right;
}

