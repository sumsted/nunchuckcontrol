#include <WiFi.h>
#include <WiFiUDP.h>
#include "nunchuck.h"

// #define WIFI_SSID "robot"
// #define WIFI_PASS "robot"
// #define SERVER_PORT 8484
// #define SERVER_IP "10.42.0.1"

#define WIFI_SSID "roveresp"
#define WIFI_PASS "roverpass"
#define SERVER_IP "192.168.4.1"
#define SERVER_PORT 80

#define HIGH_JOYSTICK_RANGE  2
#define CENTER_JOYSTICK_RANGE  1
#define LOW_JOYSTICK_RANGE  0

Nunchuck *nunchuck;
uint8_t xRange[3] = {35, 130, 225};
uint8_t yRange[3] = {26, 124, 220};
const char *payloadPattern = "%04d%04d";
WiFiUDP udp;

/*
 * Todo:
 * 1. establish wifi connection with event handlers to try to reconnect
 * 2. send udp packet
 * 3. add wifi event handler
 */
void setup(){
    Serial.begin(115200);

    Serial.println("Setup WiFi");
    wifiSetup();

    Serial.println("Centering joystick");
    nunchuck = new Nunchuck();
    centerJoystick();
}

void loop(){
    char payload[256];
    long left = 0;
    long right = 0;

    nunchuck->read();
    transform(nunchuck, &left, &right);
    Serial.print(left); Serial.print("\t");
    Serial.println(right);
    sprintf(payload, payloadPattern, left, right);
    sendPacket(payload);
    delay(100);
}

void wifiSetup(){
    WiFi.disconnect(true);
    delay(1000);
    WiFi.onEvent(wifiEventHandler);
    WiFi.begin(WIFI_SSID, WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi Connected");
    udp.begin(WiFi.localIP(), SERVER_PORT);
    Serial.print("Bind to UDP Port");
    delay(10);
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

}

void wifiEventHandler(WiFiEvent_t event){
    switch(event){
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("SYSTEM_EVENT_STA_GOT_IP");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.print("SYSTEM_EVENT_STA_DISCONNECTED");
            wifiSetup();
            break;
        default:
            break;
    }
}

void sendPacket(char *payload){
    udp.beginPacket(SERVER_IP, SERVER_PORT);
    udp.printf(payload);
    udp.endPacket();
}

void sendPayload(char *payload){
    WiFiClient client;
    if (!client.connect(SERVER_IP, SERVER_PORT)){
        delay(1);
    }
    client.print(payload);
    client.stop();
    delay(100);
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

void transform(Nunchuck *numchuck, long *left, long *right){
    long x = (long) ((double)(nunchuck->joystick.xPosition - xRange[LOW_JOYSTICK_RANGE]) / (double)(xRange[HIGH_JOYSTICK_RANGE] - xRange[LOW_JOYSTICK_RANGE]) * 100);
    long y = (long)((double)(nunchuck->joystick.yPosition - yRange[LOW_JOYSTICK_RANGE]) / (double)(yRange[HIGH_JOYSTICK_RANGE] - yRange[LOW_JOYSTICK_RANGE]) * 100);
    x = (x * 2) - 100;
    y = (y * 2) - 100;
    x = long((double)x / 20.0) * 20.0;
    y = long((double)y / 20.0) * 20.0;
    *left = (x < 0) ? long((double)y + ((double)x / 100.0 * (double)y)) : y;
    *right = (x > 0) ? long((double)y - ((double)x / 100.0 * (double)y)) : y;
    *left = (y == 0) ? x * .5 : *left;
    *right = (y == 0) ? -x * .5 : *right;
    *left = (abs(*left) > 100) ? ((*left/abs(*left))*100) : *left;
    *right= (abs(*right) > 100) ? ((*right/abs(*right))*100) : *right;
}