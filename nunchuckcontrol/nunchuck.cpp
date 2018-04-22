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
    joystick.xPosition = raw.data.xPosition;
    joystick.yPosition = raw.data.yPosition;
    joystick.zButton = ((raw.data.buttonsRaw >> 0) & 1) ? 0 : 1;
    joystick.cButton = ((raw.data.buttonsRaw  >> 1) & 1) ? 0 : 1;
    accel.xAngle = raw.data.xAccelRaw;
    accel.yAngle = raw.data.yAccelRaw;
    accel.zAngle = raw.data.zAccelRaw;
    accel.xAngle +=  ((raw.data.buttonsRaw  >> 2) & 1) ? 2 : 0;
    accel.xAngle +=  ((raw.data.buttonsRaw  >> 3) & 1) ? 1 : 0;
    accel.yAngle +=  ((raw.data.buttonsRaw  >> 4) & 1) ? 2 : 0;
    accel.yAngle +=  ((raw.data.buttonsRaw  >> 5) & 1) ? 1 : 0;
    accel.zAngle +=  ((raw.data.buttonsRaw  >> 6) & 1) ? 2 : 0;
    accel.zAngle +=  ((raw.data.buttonsRaw  >> 7) & 1) ? 1 : 0;
}

char Nunchuck::decodeByte(char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

int Nunchuck::read(){
    int i=0;
    sendCommand();
    delay(10);
    Wire.requestFrom (0x52, 6);
    while (Wire.available()) {
        raw.buffer[i] = decodeByte(Wire.read());
        i++;
    }
    transform();
    return (i>=5) ? 0 : 1;
}

void Nunchuck::print(){
    Serial.println("Raw data\n\n");
    Serial.print("Joystick: (x,y) = (");
    Serial.print(raw.data.xPosition); Serial.print(",");
    Serial.print(raw.data.yPosition); Serial.print(")\n");
    Serial.print("Accel: (x,y,z) = (");
    Serial.print(raw.data.xAccelRaw); Serial.print(",");
    Serial.print(raw.data.yAccelRaw); Serial.print(",");
    Serial.print(raw.data.zAccelRaw); Serial.print(")\n");
    Serial.print("Buttons = ");Serial.print(raw.data.buttonsRaw);Serial.print("\n");

    Serial.println("Joystick");
    Serial.print("Joystick: (x,y) = (");
    Serial.print(joystick.xPosition); Serial.print(",");
    Serial.print(joystick.xPosition); Serial.print(")\n");
    Serial.print("Buttons: (z,c) = (");
    Serial.print(joystick.zButton); Serial.print(",");
    Serial.print(joystick.cButton); Serial.print(")\n");

    Serial.print("Accel: (x,y,z) = (");
    Serial.print(accel.xAngle); Serial.print(",");
    Serial.print(accel.yAngle); Serial.print(",");
    Serial.print(accel.zAngle); Serial.print(")\n");
}

void Nunchuck::chart(){
//    Serial.print(raw.data.xPosition); Serial.print("\t");
//    Serial.print(raw.data.yPosition);   Serial.print("\t");

//    Serial.print(raw.data.xAccelRaw); Serial.print("\t");
//    Serial.print(raw.data.yAccelRaw); Serial.print("\t");
//    Serial.print(raw.data.zAccelRaw); Serial.print("\n");
//    Serial.print(raw.data.buttonsRaw);Serial.print("\n");

    Serial.print(joystick.xPosition); Serial.print("\t");
    Serial.print(joystick.yPosition); Serial.print("\t");

//    Serial.print(joystick.zButton); Serial.print("\t");
//    Serial.print(joystick.cButton); Serial.print("\n");
//
//    Serial.print(accel.xAngle); Serial.print(",");
//    Serial.print(accel.yAngle); Serial.print(",");
//    Serial.print(accel.zAngle); Serial.print(")\n");
}