#include <Wire.h>

static uint8_t nunchuckBuffer[6];   // array to store nunchuck data,

void setup()
{
  Serial.begin(19200);

  nunchuckSetup(); // send the initilization handshake
  Serial.print ("Finished setup\n");
}

void loop()
{
  nunchuckReceiveData();

  nunchuckPrintData();

}

void nunchuckSetup()
{
  Wire.begin();                      // join i2c bus as master
  Wire.beginTransmission(0x52);     // transmit to device 0x52
  Wire.write(0x40);            // sends memory address
  Wire.write(0x00);            // sends sent a zero.
  Wire.endTransmission();     // stop transmitting
}

void nunchuckRequestData()
{
  Wire.beginTransmission(0x52);     // transmit to device 0x52
  Wire.write(0x00);            // sends one byte
  Wire.endTransmission();     // stop transmitting
}

int nunchuckReceiveData()
{
    int cnt=0;
    Wire.requestFrom (0x52, 6);     // request data from nunchuck
    while (Wire.available ()) {
      // receive byte as an integer
      nunchuckBuffer[cnt] = nunchuckDecodeByte(Wire.read());
      cnt++;
    }
    nunchuckRequestData();  // send request for next data payload
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5) {
     return 1;   // success
    }
    return 0; //failure
}

void nunchuckPrintData()
{
  static int i=0;
  int joy_x_axis = nunchuckBuffer[0];
  int joy_y_axis = nunchuckBuffer[1];

  int accel_x_axis = nunchuckBuffer[2]; // * 2 * 2;
  int accel_y_axis = nunchuckBuffer[3]; // * 2 * 2;
  int accel_z_axis = nunchuckBuffer[4]; // * 2 * 2;


  int z_button = 0;
  int c_button = 0;

  // byte nunchuckBuffer[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((nunchuckBuffer[5] >> 0) & 1)
    z_button = 1;
  if ((nunchuckBuffer[5] >> 1) & 1)
    c_button = 1;

  if ((nunchuckBuffer[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((nunchuckBuffer[5] >> 3) & 1)
    accel_x_axis += 1;

  if ((nunchuckBuffer[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((nunchuckBuffer[5] >> 5) & 1)
    accel_y_axis += 1;

  if ((nunchuckBuffer[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((nunchuckBuffer[5] >> 7) & 1)
    accel_z_axis += 1;

  Serial.print(i,DEC);
  Serial.print("\t");

  Serial.print("joy:");
  Serial.print(joy_x_axis,DEC);
  Serial.print(",");
  Serial.print(joy_y_axis, DEC);
  Serial.print("  \t");

  Serial.print("acc:");
  Serial.print(accel_x_axis, DEC);
  Serial.print(",");
  Serial.print(accel_y_axis, DEC);
  Serial.print(",");
  Serial.print(accel_z_axis, DEC);
  Serial.print("\t");

  Serial.print("but:");
  Serial.print(z_button, DEC);
  Serial.print(",");
  Serial.print(c_button, DEC);

  Serial.print("\r\n");  // newline
  i++;
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuckDecodeByte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}
