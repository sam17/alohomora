#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

const char* wifiName = "Azkaban";
const char* wifiPass = "voldemort";

bool isOpen = false;
ESP8266WebServer server(80);
 
//Handles http request 
void handleRoot() {
  digitalWrite(16, 0);   //Blinks on board led on page request 
  digitalWrite(5, 0);   //Blinks on board led on page request 

  server.send(200, "text/plain", "hello from esp8266!");
}

void resetPins() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

void closeDoor() {

  // if(!isOpen) {
  //   server.send(200, "text/plain", "Door already closed.");
  //   return;
  // }
  digitalWrite(D0, LOW);
  digitalWrite(D1, HIGH);
  delay(2000);
  resetPins();

  server.send(200, "text/plain", "Close door.");
  isOpen = false;
}
 
void openDoor() {

  // if(isOpen) {
  //   server.send(200, "text/plain", "Door already open.");
  //   return;
  // }

  digitalWrite(D0, HIGH);
  digitalWrite(D1, LOW);
  // delay(250);
  // resetPins();
  
  server.send(200, "text/plain", "Open door.");
  isOpen = true;
}

void reset() {
  resetPins();
  server.send(200, "text/plaIn", "Reset Done");
  
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
    if (MDNS.begin("firang")) {  //Start mDNS with name esp8266
      Serial.println("MDNS started");
    }
  }
 
  server.on("/", handleRoot);  //Associate handler function to path
    
  server.on("/open", openDoor);  //Associate handler function to path
  server.on("/close", closeDoor);
  server.on("/reset", reset);  //Associate handler function to path
  
  server.begin();                           //Start server
  Serial.println("HTTP server started");
}
 
// the loop function runs over and over again forever
void loop() {
  server.handleClient();
}