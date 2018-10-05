#include "nunchuck.h"
#include "requests.h"


// #define ROBOT_WIFI_SSID "XXX"
// #define ROBOT_WIFI_PASSWORD "XXX"
// #define ROBOT_PROTOCOL "http"
// #define ROBOT_PORT 2807
// #define ROBOT_HOST "192.168.4.1"
// #define ROBOT_PATH "robot"
#define ROBOT_WIFI_SSID "ubiquityrobot7F3D"
#define ROBOT_WIFI_PASSWORD "robot"
#define ROBOT_PROTOCOL "http"
#define ROBOT_PORT 8484
#define ROBOT_HOST "10.42.0.1"
#define ROBOT_PATH "robot"

Nunchuck *nunchuck;
Requests *requests;

uint8_t xRange[3] = {35, 130, 225};
uint8_t yRange[3] = {26, 124, 220};

#define HIGH_JOYSTICK_RANGE  2
#define CENTER_JOYSTICK_RANGE  1
#define LOW_JOYSTICK_RANGE  0

const char *payloadPattern = "{\"left\":%d,\"right\":%d}";
char payload[256];
long left = 0;
long right = 0;


void setup()
{
    Serial.begin(19200);
    nunchuck = new Nunchuck();
    centerJoystick();
    ConfigurationUnion *pcu = defaultConfig();
    requests = new Requests(pcu);
}

void loop()
{
    nunchuck->read();
    transform();
    Serial.print(left); Serial.print("\t");
    Serial.print(right); Serial.print("\t");
    Serial.print(nunchuck->joystick.cButton*10); Serial.print("\t");
    Serial.print(nunchuck->joystick.zButton*10); Serial.print("\n");
    sprintf(payload, payloadPattern, left, right);
    requests->sendUDP(payload);
    delay(250);
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

ConfigurationUnion *defaultConfig(){
    ConfigurationUnion *cu = new ConfigurationUnion;
    memset(cu, '\0', sizeof(cu));
    strcpy(cu->configuration.serial, "RB001");
    strcpy(cu->configuration.deviceId, "ROBOT01");
    strcpy(cu->configuration.model, "ESP32");
    strcpy(cu->configuration.firmware, "1.0");
    strcpy(cu->configuration.wifiSsid, ROBOT_WIFI_SSID);
    strcpy(cu->configuration.wifiPassword, ROBOT_WIFI_PASSWORD);
    strcpy(cu->configuration.location, "Home");
    strcpy(cu->configuration.gatewayProtocol,ROBOT_PROTOCOL);
    strcpy(cu->configuration.gatewayHost,ROBOT_HOST);
    cu->configuration.gatewayPort = ROBOT_PORT;
    strcpy(cu->configuration.gatewayPath,ROBOT_PATH);
    strcpy(cu->configuration.gatewayDeviceKey, "");
    strcpy(cu->configuration.state[1],"1");
    strcpy(cu->configuration.state[2],"2");
    strcpy(cu->configuration.state[3],"3");
    Serial.println("default config initialized");
    return cu;
}