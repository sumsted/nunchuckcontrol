#include <ros.h>
#include <std_msgs/String.h>
#include <breadcrumbs/Mpu.h>
#include "quaternionFilters.h"
#include "MPU9250.h"

#define AHRS true         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging

// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins

MPU9250 myIMU;

ros::NodeHandle  nh;
std_msgs::String ack;
breadcrumbs::Mpu mpuMessage;

ros::Publisher mpu("mpu", &mpuMessage);

void setup()
{
    Wire.begin();
    // TWBR = 12;  // 400 kbit/sec I2C speed
    Serial.begin(38400);

    // Set up the interrupt pin, its set as active high, push-pull
    pinMode(intPin, INPUT);
    digitalWrite(intPin, LOW);
    initMpu();
    nh.initNode();
    nh.advertise(mpu);
}

void loop()
{
    nh.spinOnce();
    checkMpu();
    writeSensorData();
}

void writeSensorData(){
    mpuMessage.yaw = myIMU.yaw;
    mpuMessage.pitch = myIMU.pitch;
    mpuMessage.roll = myIMU.roll;
    mpuMessage.rate = (float)myIMU.sumCount/myIMU.sum;
    mpu.publish(&mpuMessage);
}

void initMpu(){
    // byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    myIMU.MPU9250SelfTest(myIMU.SelfTest);
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    myIMU.initMPU9250();
    // byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    myIMU.initAK8963(myIMU.magCalibration);
}

void checkMpu(){
    if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {  
        myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
        myIMU.getAres();

        myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
        myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
        myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];

        myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
        myIMU.getGres();

        myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
        myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
        myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

        myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values
        myIMU.getMres();
        myIMU.magbias[0] = +470.;
        myIMU.magbias[1] = +120.;
        myIMU.magbias[2] = +125.;

        myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes*myIMU.magCalibration[0] -
        myIMU.magbias[0];
        myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes*myIMU.magCalibration[1] -
        myIMU.magbias[1];
        myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes*myIMU.magCalibration[2] -
        myIMU.magbias[2];
    } 

    myIMU.updateTime();

    MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx*DEG_TO_RAD,
        myIMU.gy*DEG_TO_RAD, myIMU.gz*DEG_TO_RAD, myIMU.my,
        myIMU.mx, myIMU.mz, myIMU.deltat);

    myIMU.delt_t = millis() - myIMU.count;

    if (myIMU.delt_t > 500)
    {
        myIMU.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
        *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
        - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
        myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
        *(getQ()+2)));
        myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
        *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
        - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
        myIMU.pitch *= RAD_TO_DEG;
        myIMU.yaw   *= RAD_TO_DEG;
        myIMU.yaw   += 1.5;
        myIMU.roll  *= RAD_TO_DEG;
        myIMU.count = millis();
        myIMU.sumCount = 0;
        myIMU.sum = 0;
    } 
}