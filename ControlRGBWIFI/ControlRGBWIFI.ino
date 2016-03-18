#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <aJSON.h>

//Include the HTML, STYLE and Script "Pages"
#include "index_html.h"
#include "index_css.h"
#include "index_js.h"

// Create an instance of the server specify the port to listen on as an argument
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1234, "", "mensaje_vortex");
int ms_ultima_medicion_memoria = 0;
String numeroRecibido = "";
int rojo = 0;
int verde = 0;
int azul = 0;

int pinRojo = 13;
int estadoPinRojo = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            }
            break;
        case WStype_TEXT:
            for(int i=1; i<lenght; i++){
                numeroRecibido += payload[i];
            } 

            if(payload[0]=='R') rojo = numeroRecibido.toInt();
            if(payload[0]=='G') verde = numeroRecibido.toInt();
            if(payload[0]=='B') azul = numeroRecibido.toInt();

            //Serial.println("R" + rojo + "G" + verde + "V" + azul);
            Serial.printf("R%d G%d B%d\n", rojo, verde, azul);
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            break;
    }
    
}

//----------------------------------------------------------------------- 
void handleIndex_html(){
  Serial.println("me piden html");
  server.send(200,"text/html",PAGE_index_html);
}

//----------------------------------------------------------------------- 
void handleIndex_css(){
  Serial.println("me piden css");
  server.send(200,"text/css",PAGE_index_css);
}

//----------------------------------------------------------------------- 
void handleIndex_js(){
  Serial.println("me piden js");
  server.send(200,"text/js",PAGE_index_js);
}

//----------------------------------------------------------------------- 
void handleNotFound() {
  Serial.println("me piden cualquiera");
  server.send(404,"text/html","no lo seeee");
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
   server.on("/index.html",handleIndex_html);
   server.on("/index.css",handleIndex_css);
   server.on("/index.js",handleIndex_js);
   
   server.onNotFound ( handleNotFound );

   if (MDNS.begin("CamaraMultiplano")) {
    Serial.println("MDNS responder started");
  }
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println ( "WSockets server started" );

  pinMode(pinRojo, OUTPUT);
  digitalWrite(pinRojo, 1);
  estadoPinRojo = 1;
}
//----------------------------------------------------------------------- 
void loop(){
  server.handleClient();
  webSocket.loop();
  
  if((millis()-ms_ultima_medicion_memoria)>5000){
    Serial.println(ESP.getFreeHeap());
    ms_ultima_medicion_memoria = millis();
    estadoPinRojo = 1 - estadoPinRojo;
    digitalWrite(pinRojo, estadoPinRojo);
  }
}
