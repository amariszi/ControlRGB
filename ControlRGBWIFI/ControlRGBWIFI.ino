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
int ms_ultimo_envio = 0;
String numeroRecibido = "";

uint8_t recibidosSerie[20];
int iSerie = 0;

int rojo = 0;
int verde = 0;
int azul = 0;

int corr_rojo = 0;
int corr_verde = 0;
int corr_azul = 0;

int pinRojo = 14;
int pinVerde = 12;
int pinAzul = 13;

int pinRojo2 = 4;
int pinVerde2 = 5;
int pinAzul2 = 15;

int estadoPinRojo = 0;

void enviarATodosMenosA(uint8_t excluido, String dato) {
  for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    if (i != excluido) webSocket.sendTXT(i, dato);
  }
}

void handleDato(uint8_t * payload, size_t lenght){
  numeroRecibido = "";
  char inbyte;
  inbyte = payload[0];
  //Serial.println("recibo:");
  //Serial.println(inbyte);
  for (int i = 1; i < lenght; i++) {
	inbyte = payload[i];
	numeroRecibido += inbyte;
	//Serial.println(inbyte);
  }
  //Serial.println(numeroRecibido);
  inbyte = payload[0];
  if (inbyte == 'R') {
	rojo = numeroRecibido.toInt();
	analogWrite(pinRojo, map(rojo, 0, 255, 0, 1023));
	int rojo_corregido = rojo + corr_rojo;
	if (rojo_corregido < 0) rojo_corregido = 0;
	if (rojo_corregido > 255) rojo_corregido = 255;
	analogWrite(pinRojo2, map(rojo_corregido, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "R"+ String(rojo));
  }
  if (inbyte == 'G') {
	verde = numeroRecibido.toInt();
	analogWrite(pinVerde, map(verde, 0, 255, 0, 1023));
	int verde_corregido = verde + corr_verde;
	if (verde_corregido < 0) verde_corregido = 0;
	if (verde_corregido > 255) verde_corregido = 255;
	analogWrite(pinVerde2, map(verde_corregido, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "G"+ String(verde));
  }
  if (inbyte == 'B') {
	azul = numeroRecibido.toInt();
	analogWrite(pinAzul, map(azul, 0, 255, 0, 1023));
	int azul_corregido = azul + corr_azul;
	if (azul_corregido < 0) azul_corregido = 0;
	if (azul_corregido > 255) azul_corregido = 255;
	analogWrite(pinAzul2, map(azul_corregido, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "B"+ String(azul));
  }

  if (inbyte == 'r') {
	corr_rojo = numeroRecibido.toInt();
	analogWrite(pinRojo2, map(rojo + corr_rojo, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "r"+ String(corr_rojo));
  }
  if (inbyte == 'g') {
	corr_verde = numeroRecibido.toInt();
	analogWrite(pinVerde2, map(verde + corr_verde, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "g"+ String(corr_verde));
  }
  if (inbyte == 'b') {
	corr_azul = numeroRecibido.toInt();
	analogWrite(pinAzul2, map(azul + corr_azul, 0, 255, 0, 1023));
	//enviarATodosMenosA(num, "b"+ String(corr_azul));
  }

  Serial.print("panel 1:");
  Serial.println("R"+String(rojo)+"G"+String(verde)+"B"+String(azul));
  Serial.print("panel 2:");
  Serial.println("R"+String(rojo+corr_rojo)+"G"+String(verde+corr_verde)+"B"+String(azul+corr_azul));
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "R" + String(rojo));
        webSocket.sendTXT(num, "G" + String(verde));
        webSocket.sendTXT(num, "B" + String(azul));
        webSocket.sendTXT(num, "r" + String(corr_rojo));
        webSocket.sendTXT(num, "g" + String(corr_verde));
        webSocket.sendTXT(num, "b" + String(corr_azul));
      }
      break;
    case WStype_TEXT:
      handleDato(payload, lenght);
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
      break;
  }

}

//-----------------------------------------------------------------------
void handleIndex_html() {
  //Serial.println("me piden html");
  server.send(200, "text/html", PAGE_index_html);
}

//-----------------------------------------------------------------------
void handleIndex_css() {
  //Serial.println("me piden css");
  server.send(200, "text/css", PAGE_index_css);
}

//-----------------------------------------------------------------------
void handleIndex_js() {
  //Serial.println("me piden js");
  server.send(200, "text/js", PAGE_index_js);
}

//-----------------------------------------------------------------------
void handleNotFound() {
  //Serial.println("me piden cualquiera");
  server.send(200, "text/html", PAGE_index_html);
}

//-----------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Camara_Multiplano");

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  // Start the server
  server.on("/index.html", handleIndex_html);
  server.on("/index.css", handleIndex_css);
  server.on("/index.js", handleIndex_js);
  
  server.onNotFound ( handleNotFound );

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println ( "WSockets server started" );

  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
}
//-----------------------------------------------------------------------
void loop() {
	server.handleClient();
	webSocket.loop();

//	if ((millis() - ms_ultima_medicion_memoria) > 5000) {
//		Serial.println(ESP.getFreeHeap());
//		ms_ultima_medicion_memoria = millis();
//	}
	
	if(Serial.available() > 0){
    int recibido = Serial.read();
    if(recibido==13){
      if(iSerie >0) handleDato(recibidosSerie, iSerie);
      iSerie=0;
    }else{
      recibidosSerie[iSerie] = recibido;
      iSerie++;
    }
	}
}







