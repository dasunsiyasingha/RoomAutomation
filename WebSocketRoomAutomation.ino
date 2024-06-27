/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "index.h"

const char* ssid = "dassamobi";     // CHANGE IT
const char* password = "12345678";  // CHANGE IT

ESP8266WebServer server(80);                        // Web server on port 80
WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

void broadcastItemState(const String item, const String state ) {
    StaticJsonDocument<200> doc;
    JsonArray array = doc.to<JsonArray>();
    Serial.println("add items to array: ");
    Serial.println(item);
    Serial.println(state);

    array.add(item);
    array.add(state);

    String message;
    serializeJson(array, message);
    Serial.println(message);

    webSocket.broadcastTXT(message);
}

String allItemState[][2] = {
  {"item1", "off"},
  {"item2", "off"},
  {"item3", "off"},
  {"item4", "off"},
  {"item5", "off"},
  {"item6", "off"}
};

void itemsOnOff(const String itemnum, const String state){
  if(itemnum == "item1"){
      if(state == "on"){
        Serial.println("Light is On");
        allItemState[0][1] = state;
      }else{
        Serial.println("Light is Off");
        allItemState[0][1] = state;
      }
  }else if(itemnum == "item2"){
      if(state == "on"){
          Serial.println("night light is On");
          allItemState[1][1] = state;
      }else{
          Serial.println("night light is Off");
          allItemState[1][1] = state;
      }
  }else if(itemnum == "item3"){
      if(state == "on"){
          Serial.println("FAN is On");
          allItemState[2][1] = state;
      }else{
          Serial.println("FAN is Off");
          allItemState[2][1] = state;
      }
  }else if(itemnum == "item4"){
      if(state == "on"){
          Serial.println("Plug1 is On");
          allItemState[3][1] = state;
      }else{
          Serial.println("Plug1 is Off");
          allItemState[3][1] = state;
      }
  }else if(itemnum == "item5"){
      if(state == "on"){
          Serial.println("Plug2 is On");
          allItemState[4][1] = state;
      }else{
          Serial.println("Plug2 is Off");
          allItemState[4][1] = state;
      }
  }else if(itemnum == "item6"){
      if(state == "on"){
          Serial.println("Plug3 is On");
          allItemState[5][1] = state;
      }else{
          Serial.println("Plug3 is Off");
          allItemState[5][1] = state;
      }
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        for(int i = 0; i<6; i++){
          broadcastItemState(allItemState[i][0],allItemState[i][1]);
          itemsOnOff(allItemState[i][0],allItemState[i][1]);
        }

      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", num, payload);

      StaticJsonDocument<200> doc;
      DeserializationError error  = deserializeJson(doc, payload);

      if(!error){
        JsonArray array = doc.as<JsonArray>();
        Serial.print("Received Array: ");

        if(array.size()==2){
          String itemnum = array[0].as<String>();
          String state = array[1].as<String>();
          Serial.print("Item number: ");
          Serial.println(itemnum);
          Serial.print("State: ");
          Serial.println(state);

          broadcastItemState(itemnum, state);
          itemsOnOff(itemnum, state);
        }
        Serial.println();
      }else{
        Serial.println("Failed to parse JSON");
      }
      // Send a response back to the client
      // String echoMessage =  String((char*)payload);
      // webSocket.broadcastTXT(echoMessage);
      
      break;
  }
}

void setup() {
  Serial.begin(9600);
  delay(500);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Serve a basic HTML page with JavaScript to create the WebSocket connection
  server.on("/", HTTP_GET, []() {
    Serial.println("Web Server: received a web page request");
    String html = HTML_CONTENT;  // Use the HTML content from the index.h file
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.print("ESP8266 Web Server's IP address: ");
  Serial.println(WiFi.localIP());
}

int prvstate = 0;

void loop() {
  // Handle client requests
  server.handleClient();

  // Handle WebSocket events
  webSocket.loop();

  if (Serial.available() > 0) {
    // Read the incoming byte:
    int input1 = Serial.parseInt();
    delay(100);
    Serial.print("Your input value: ");
    Serial.println(input1);

    if(prvstate != input1){  //Catch event of Switch

      if(allItemState[0][1] == "off"){
        broadcastItemState("item1","on");
        itemsOnOff("item1","on");
        prvstate = input1;
      }else{
        broadcastItemState("item1","off");
        itemsOnOff("item1","off");
        prvstate = input1;
      }
    }


  }


//LAST UPDATE
  // TODO: Your code here
}
