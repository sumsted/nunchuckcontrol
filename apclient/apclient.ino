#include <WiFi.h>

void setup(){

  Serial.begin(115200);

  //reset the wifi connection, solves bugs
  WiFi.disconnect(true);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  const char* ssid = "TestingWeMos";
  const char* pass = "something";
  const uint16_t serverPort = 80;
  const char* serverIP = "192.168.4.1";

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  Serial.println("WiFi Connected");
  delay(10);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println("Connected to WiFi");
}

void loop(){

  const char *sensor_data = "hello";

  const uint16_t serverPort = 80;
  const char* serverIP = "192.168.4.1";

  WiFiClient client;

  //connect to other WEMOS
  if (!client.connect(serverIP, serverPort)){
    delay(1);
  }
  //Write data to other wemos
  client.print(sensor_data);

  //End connection
  client.stop();

  delay(100);
}