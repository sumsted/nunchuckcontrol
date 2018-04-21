#include "nunchuck.h"

Nunchuck *nunchuck;
void setup()
{
    Serial.begin(19200);
    nunchuck = new Nunchuck();
}

void loop()
{
    nunchuck->read();
    nunchuck->chart();
    delay(20);
}

