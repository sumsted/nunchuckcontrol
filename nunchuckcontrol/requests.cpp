#include "Requests.h"


Requests::Requests(){
//    readEepromConfig();
    establishWifiConnection();
}

Requests::Requests(ConfigurationUnion *newConfig){
//    writeNewConfig(newConfig);
//    readEepromConfig();
    config = newConfig;
    establishWifiConnection();
}

int Requests::sendUDP(char *payload){
    WiFiUDP udp;
    udp.beginPacket(config->configuration.gatewayHost, config->configuration.gatewayPort);
    udp.printf(payload);
    udp.endPacket();
    return 0;
}

int Requests::postEvent(int value){
    HTTPClient http;
    char payload[1000];
    char url[1000];
    const char *payloadPattern = "{\"device\":\"%s\",\"value\":%d}";
    const char *urlPattern = "%s://%s:%d/%s/%s"; // protocol://host:port/path/key
    int code;

    sprintf(payload, payloadPattern, config->configuration.deviceId, value);
    sprintf(url, urlPattern,
        config->configuration.gatewayProtocol,
        config->configuration.gatewayHost,
        config->configuration.gatewayPort,
        config->configuration.gatewayPath,
        config->configuration.gatewayDeviceKey);
    Serial.print("URL: ");
    Serial.println(url);
    Serial.print("payload: ");
    Serial.println(payload);

    if(strcmp(config->configuration.gatewayProtocol,"https")==0){
        http.begin(url, rootCa);
    } else {
        http.begin(url);
    }
    
    http.addHeader("Content-Type", "application/json");
    code = http.POST(payload);

    if(code > 0){
        String response = http.getString();
        Serial.println(code);
        Serial.println(response);
    }else{
        Serial.print("problem posting to " );
        Serial.print(config->configuration.gatewayHost);
        Serial.print(": ");
        Serial.println(code);
        Serial.print(": ");
        Serial.println(http.errorToString(code));
        Serial.print("url: ");
        Serial.println(url);
        Serial.print("payload: ");
        Serial.println(payload);
    }
    http.end();
}


int Requests::getEvent(int value){
    HTTPClient http;
    char payload[1000];
    char url[1000];
    const char *urlPattern = "%s://%s:%d/%s/%d"; // protocol://host:port/path/value
    int code;

    sprintf(url, urlPattern,
        config->configuration.gatewayProtocol,
        config->configuration.gatewayHost,
        config->configuration.gatewayPort,
        config->configuration.gatewayPath,
        value);
    Serial.print("URL: ");
    Serial.println(url);


//    if(strcmp(config->configuration.gatewayProtocol,"https")==0){
//        http.begin(url, rootCa);
//    } else {
        http.begin(url);
//    }

    code = http.GET();

    if(code > 0){
        String response = http.getString();
        Serial.println(code);
        Serial.println(response);
    }else{
        Serial.print("problem posting to " );
        Serial.print(config->configuration.gatewayHost);
        Serial.print(": ");
        Serial.println(code);
        Serial.print(": ");
        Serial.println(http.errorToString(code));
        Serial.print("url: ");
        Serial.println(url);
    }
    http.end();
}


void Requests::establishWifiConnection(){
    wifiConnected = 0;
    byte tries=0;
    Serial.print("establishwificonnection: ");
    Serial.print(config->configuration.wifiSsid);
    Serial.print(", ");
    Serial.println(config->configuration.wifiPassword);
    WiFi.begin(config->configuration.wifiSsid, config->configuration.wifiPassword);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print("attempting to connect, try: ");
        Serial.println(tries);
        tries++;
        if(tries > 10){
            Serial.print("failed to establish connection to wifi: ");
            Serial.print(config->configuration.wifiSsid);
            Serial.print(", password: ");
            Serial.println(config->configuration.wifiPassword);
        }
        delay(500);
    }
    Serial.print("connected to wifi with IP address: ");
    localIp = WiFi.localIP();
    char ipString[250];
    unsigned char part0 = localIp;
    unsigned char part1 = localIp >> (8);
    unsigned char part2 = localIp >> (16);
    unsigned char part3 = localIp >> (24);
    sprintf(ipString, "%d.%d.%d.%d", part0, part1, part2, part3);
    Serial.println(ipString);
    wifiConnected = 1;
}

void Requests::printConfig(const char *label, ConfigurationUnion *config){
    Serial.print("\nConfiguration: ");
    Serial.println(label);
    Serial.print("serial: ");
    Serial.println(config->configuration.serial);
    Serial.print("deviceId: ");
    Serial.println(config->configuration.deviceId);
    Serial.print("model: ");
    Serial.println(config->configuration.model);
    Serial.print("firmware: ");
    Serial.println(config->configuration.firmware);
    Serial.print("location: ");
    Serial.println(config->configuration.location);
    Serial.print("gatewayProtocol: ");
    Serial.println(config->configuration.gatewayProtocol);
    Serial.print("gatewayHost: ");
    Serial.println(config->configuration.gatewayHost);
    Serial.print("gatewayPort: ");
    Serial.println(config->configuration.gatewayPort);
    Serial.print("gatewayPath: ");
    Serial.println(config->configuration.gatewayPath);
    Serial.print("State 0: ");
    Serial.println(config->configuration.state[0]);
    Serial.print("State 1: ");
    Serial.println(config->configuration.state[1]);
    Serial.print("State 2: ");
    Serial.println(config->configuration.state[2]);
    Serial.print("State 3: ");
    Serial.println(config->configuration.state[3]);
}