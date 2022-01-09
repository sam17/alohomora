// #include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "battery_monitor.cpp"
#include "OTA.cpp"

#define BAUD_RATE 9600

bool debug = false;

ESP8266WebServer server(80);

void handleRoot()
{
  digitalWrite(17, 0); //Blinks on board led on page request
  digitalWrite(5, 0);  //Blinks on board led on page request
  server.send(200, "text/plain", "Hello from Alohomora!");
}

void initDoorPins()
{
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
}

void resetPins()
{
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

void closeDoor()
{
  digitalWrite(D0, LOW);
  digitalWrite(D1, HIGH);
  // resetPins();
  server.send(200, "text/plain", "Close door.");
}

void openDoor()
{
  digitalWrite(D0, HIGH);
  digitalWrite(D1, LOW);
  // closeDoor();
  server.send(200, "text/plain", "Open door.");
}

void reset()
{
  resetPins();
  server.send(200, "text/plaIn", "Reset Done");
}

void door_loop() {
  if(debug) {
    Serial.println("Status DO D1:");
    Serial.println(digitalRead(D0));
    Serial.println(digitalRead(D1));
  }
}

void app_loop()
{
  server.handleClient();
}

void app_server_init()
{
  const char *wifiName = "Azkaban";
  const char *wifiPass = "voldemort";
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiName);
  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());    //You can get IP address assigned to ESP
  if (WiFi.status() == WL_CONNECTED) //If WiFi connected to hot spot then start mDNS
  {
    Serial.println(WiFi.status());
    if (MDNS.begin("alohomora"))
    { //Start mDNS with name esp8266
      Serial.println("MDNS started");
    }

    server.on("/", handleRoot);   //Associate handler function to path
    server.on("/open", openDoor); //Associate handler function to path
    server.on("/close", closeDoor);
    server.on("/reset", reset); //Associate handler function to path
    server.begin();             //Start server
    Serial.println("HTTP server started");
  }
}

BatteryMonitor batteryMonitor = BatteryMonitor(A0, debug);
OverTheAir overTheAir;
// the setup function runs once when you press reset or power the board
void setup()
{

  Serial.begin(BAUD_RATE);
  app_server_init();
  batteryMonitor.setup();
  overTheAir.startOTA();
  delay(10);
}

// the loop function runs over and over again forever
void loop()
{
  batteryMonitor.loop();
  overTheAir.ota_loop();
  app_loop();
  door_loop();
  delay(50);
}