#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Include the HTML, STYLE and Script "Pages"
#include "index.h"

// Create an instance of the server specify the port to listen on as an argument
ESP8266WebServer server(80);


//----------------------------------------------------------------------- 
void handleIndex(){
  Serial.println("me piden index");
  server.send(200,"text/html",PAGE_index);
}

//----------------------------------------------------------------------- 
void handleNotFound() {
  Serial.println("me piden cualquiera");
  server.send(200,"text/html",PAGE_index);
}
//-----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Camara_Multiplano");

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  // Start the server
   server.on("/",handleIndex);
   server.onNotFound ( handleNotFound );
//
//   if (MDNS.begin("CamaraMultiplano")) {
//    Serial.println("MDNS responder started");
//  }
  server.begin();
}
//----------------------------------------------------------------------- 
void loop(){
   server.handleClient();
}
