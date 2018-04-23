#include <WiFi.h>

//Server at port 80
WiFiServer server(80);
RoverMotor *roverMotor;

void setup(){
  Serial.begin(115200);
    roverMotor = new RoverMotor();
  //Resets the wifi connection, solved a bug where no communication took place between two wemos's
  WiFi.disconnect(true);
  delay(1000);
  const char* ssid = "TestingWeMos";
  const char* pass = "something";

  WiFi.softAP(ssid, pass);
  Serial.println(WiFi.softAPIP());
  server.begin();
  Serial.println("Server started");
}

void loop(){

  //Check for client connections
  WiFiClient client = server.available();

  if(!client){
    //No client detected
    Serial.println("No client");
    delay(100);
    return;
  }

  Serial.println("Client has connected");
  //wait for data
  while(!client.available()){
    //Client connected, no data sent
    delay(1);
  }

  String message = client.readStringUntil('\r');
  Serial.println(message);
  client.flush();
}