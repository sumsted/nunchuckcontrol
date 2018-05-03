#include <WiFi.h>
#include <WiFiUdp.h>
#include "rovermotor.h"

#define WIFI_SSID "roveresp"
#define WIFI_PASS "roverpass"
#define SERVER_PORT 80

//RoverMotor *roverMotor;
WiFiUDP udp;

/*
 * todo:
 * 1. establish wifi access point
 * 2. bind to port for udp
 * 3. loop looking for packet
 * 4. process packet
 */

void setup(){
    Serial.begin(115200);
//    roverMotor = new RoverMotor();

    WiFi.disconnect(true);
    delay(1000);
    WiFi.softAP(WIFI_SSID, NULL);
    Serial.println(WiFi.softAPIP());
    udp.begin(SERVER_PORT); //bind to port
    Serial.println("Listening for UDP packets");
}

void loop(){
    long left = 0;
    long right = 0;
    if(udpPacketHandler(&left, &right)){
        Serial.print(left); Serial.print("\t");
        Serial.println(right);
//        roverMotor->runMotor(left, right);
    }
//    roverMotor->testLR();
}

boolean udpPacketHandler(long *left, long *right){
    char buffer[50];
    if(udp.parsePacket()){
        if(udp.read(buffer, sizeof(buffer))){
            parsePayload(buffer, left, right);
            return true;
        }
    }
    return false;
}

void parsePayload(String  payload, long *left, long *right){
    if(payload.length()==8){
        *left = payload.substring(0, 4).toInt();
        *right = payload.substring(4, 8).toInt();
    }
}
