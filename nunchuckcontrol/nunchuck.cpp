#include "nunchuck.h"


Nunchuck::Nunchuck(){
    Wire.begin();
    Wire.beginTransmission(0x52);
    Wire.write(0x40); //Todo ?
    Wire.write(0x00);
    Wire.endTransmission();
}

void Nunchuck::sendCommand(){
    Wire.beginTransmission(0x52);
    Wire.write(0x00);
    Wire.endTransmission();
}

void Nunchuck::transform(){
    joystick.xPosition = data.xPosition;
    joystick.yPosition = data.yPosition;
    joystick.zButton = ((nunchuckBuffer[5] >> 0) & 1) ? 1 : 0;
    joystick.cButton = ((nunchuckBuffer[5] >> 1) & 1) ? 1 : 0;
    accel.xAngle +=  ((nunchuckBuffer[5] >> 2) & 1) ? 2 : 0;
    accel.xAngle +=  ((nunchuckBuffer[5] >> 3) & 1) ? 1 : 0;
    accel.yAngle +=  ((nunchuckBuffer[5] >> 4) & 1) ? 2 : 0;
    accel.yAngle +=  ((nunchuckBuffer[5] >> 5) & 1) ? 1 : 0;
    accel.zAngle +=  ((nunchuckBuffer[5] >> 6) & 1) ? 2 : 0;
    accel.zAngle +=  ((nunchuckBuffer[5] >> 7) & 1) ? 1 : 0;
}

void read(){
    int i=0;
    Wire.requestFrom (0x52, 6);
    while (Wire.available()) {
        data[i] = nunchuckDecodeByte(Wire.read());
        i++;
    }
    return (i>=5) ? 0 : 1;
}