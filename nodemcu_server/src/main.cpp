#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>

const char* wifiName = "Azkaban";
const char* wifiPass = "voldemort";

ESP8266WebServer server(80);
 
//Handles http request 
void handleRoot() {
  digitalWrite(16, 0);   //Blinks on board led on page request 
  digitalWrite(5, 0);   //Blinks on board led on page request 

  server.send(200, "text/plain", "Hello from Alohomora!");
}

void resetPins() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

void closeDoor() {

  digitalWrite(D0, LOW);
  digitalWrite(D1, HIGH);
  delay(2000);
  resetPins();

  server.send(200, "text/plain", "Close door.");
}
 
void openDoor() {

  digitalWrite(D0, HIGH);
  digitalWrite(D1, LOW);
  delay(10000);
  closeDoor();

  server.send(200, "text/plain", "Open door.");
}

void reset() {
  resetPins();
  server.send(200, "text/plaIn", "Reset Done");
  
}

void StartOTA() {

  ArduinoOTA.setHostname("alohomora");
 ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(9600);

  Serial.println("Setting D0 and D1");
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  closeDoor();
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(wifiName);
 
  WiFi.begin(wifiName, wifiPass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
 
   if(WiFi.status() == WL_CONNECTED) //If WiFi connected to hot spot then start mDNS
  {
    Serial.println(WiFi.status());
    // if (MDNS.begin("firang")) {  //Start mDNS with name esp8266
    //   Serial.println("MDNS started");
    // }
  }
 
  StartOTA();
  server.on("/", handleRoot);  //Associate handler function to path
    
  server.on("/open", openDoor);  //Associate handler function to path
  server.on("/close", closeDoor);
  server.on("/reset", reset);  //Associate handler function to path
  
  server.begin();                           //Start server
  Serial.println("HTTP server started");
}


 
// the loop function runs over and over again forever
void loop() {
   ArduinoOTA.handle();
  server.handleClient();
}