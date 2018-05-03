//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "XXX";
const char* password = "XXX";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2807;

const int packetSize = 2;
byte packetBuffer[packetSize];

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
//  WiFi.config(IPAddress(192, 168, 1, 60), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  UDPTestServer.begin(UDPPort);

}

int value = 0;

void loop() {
   handleUDPServer();
   delay(1);
}

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = "";
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.println(myData);
  }
}