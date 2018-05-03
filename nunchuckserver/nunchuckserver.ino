#include <WiFi.h>
#include <WiFiUdp.h>
#include "rovermotor.h"

#define WIFI_SSID "roveresp"
#define WIFI_PASS "roverpass"
#define SERVER_PORT 80

RoverMotor *roverMotor;
WiFiUDP udp;

void setup(){
    Serial.begin(115200);
   roverMotor = new RoverMotor();

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
       roverMotor->runMotor(left, right);
    }
//    roverMotor->testLR();
}

boolean udpPacketHandler(long *left, long *right){
    char buffer[50];
    int i;
    if(udp.parsePacket()){
        memset(buffer, '\0', sizeof(buffer));
        if(i=udp.read(buffer, sizeof(buffer))){
            Serial.print("buffer(");Serial.print(i);Serial.print("):");
            Serial.println(buffer);
            return parsePayload(buffer, left, right);
        }
    }
    return false;
}

boolean parsePayload(char *payload, long *left, long *right){
    int i = strlen(payload);
    char cnum[5];
    memset(cnum, '\0', sizeof(cnum));
    if(i==8){
        strncpy(cnum, payload, 4);
        *left = atoi(cnum);
        strncpy(cnum, payload+4, 4);
        *right = atoi(cnum);
        return true;
    }
    return false;
}
