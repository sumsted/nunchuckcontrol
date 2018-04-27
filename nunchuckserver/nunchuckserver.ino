#include <WiFi.h>
#include "rovermotor.h"

#define WIFI_SSID "roveresp"
#define WIFI_PASS "roverpass"
#define SERVER_PORT 80

RoverMotor *roverMotor;
WiFiServer *server;

void parsePayload(String  payload, long *left, long *right){
    if(payload.length()==8){
        *left = payload.substring(0, 4).toInt();
        *right = payload.substring(4, 8).toInt();
    }
}

int wifiHandler(long *left, long *right){
    WiFiClient client = server->available();
    if(!client){
        Serial.println("No client");
        delay(100);
        return -1;
    }

    Serial.println("Waiting for data");
    while(!client.available()){
        delay(1);
    }

    String payload = client.readStringUntil('\r');
    Serial.println("Data read");
    Serial.println(payload);
    client.flush();
    parsePayload(payload, left, right);
    return 0;
}

void setup(){
    Serial.begin(115200);
    roverMotor = new RoverMotor();

//    server = new WiFiServer(80);
//    WiFi.disconnect(true);
//    delay(1000);
//    WiFi.softAP(WIFI_SSID, WIFI_PASS);
//    Serial.println(WiFi.softAPIP());
//    server->begin();
//    Serial.println("Server started");
}

void loop(){
//    long left = 0;
//    long right = 0;
//    if(!wifiHandler(&left, &right)){
//        roverMotor->runMotor(left, right);
//    }
    roverMotor->testLR();
}