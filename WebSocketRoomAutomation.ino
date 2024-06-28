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



String allItemState[][2] = {
  {"item1", "off"},
  {"item2", "off"},
  {"item3", "off"},
  {"item4", "off"},
  {"item5", "off"},
  {"item6", "off"}
};

String RGBvalues[3][2] = {
  {"r", "0"},
  {"g", "0"},
  {"b", "0"}
};

//Switches: GPIO 0, GPIO 2, GPIO 4, GPIO 5, GPIO 12, GPIO 13
const int switch1pin = 0;
const int switch2pin = 2;
const int switch3pin = 4;
const int switch4pin = 5;
const int switch5pin = 12;

const int switch6pin = 13;

//Relays: GPIO 16, GPIO 15, GPIO 14, GPIO 10, GPIO 9,             GPIO 3, GPIO 1, GPIO 7
const int relay1 = 16;
const int relay2 = 15;
const int relay3 = 14;
const int relay4 = 10;
const int relay5 = 9;

// const int relay5 = 3;
// const int relay6 = 1;
// const int relay7 = 7;
//GPIO 3, 1, 7 are cannot use..

void broadcastRGBvalue(const String rgbarray[][2], size_t arraySize){
  StaticJsonDocument<200> doc;
  JsonArray array = doc.to<JsonArray>();

  for (size_t i = 0; i < arraySize; i++) {
    JsonArray item = array.createNestedArray();
    item.add(rgbarray[i][0]);
    item.add(rgbarray[i][1]);
  }

  String message;
  serializeJson(array, message);
  Serial.println(message);
  webSocket.broadcastTXT(message);
}

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

void itemsOnOff(const String itemnum, const String state){
  if(itemnum == "item1"){
      if(state == "on"){
        Serial.println("Light is On");
        allItemState[0][1] = state;
        digitalWrite(relay1, HIGH);
      }else{
        Serial.println("Light is Off");
        allItemState[0][1] = state;
        digitalWrite(relay1, LOW);
      }
  }else if(itemnum == "item2"){
      if(state == "on"){
          Serial.println("night light is On");
          allItemState[1][1] = state;
          digitalWrite(relay2, HIGH);
      }else{
          Serial.println("night light is Off");
          allItemState[1][1] = state;
          digitalWrite(relay2, LOW);
      }
  }else if(itemnum == "item3"){
      if(state == "on"){
          Serial.println("FAN is On");
          allItemState[2][1] = state;
          digitalWrite(relay3, HIGH);
      }else{
          Serial.println("FAN is Off");
          allItemState[2][1] = state;
          digitalWrite(relay3, LOW);
      }
  }else if(itemnum == "item4"){
      if(state == "on"){
          Serial.println("Plug1 is On");
          allItemState[3][1] = state;
          digitalWrite(relay4, HIGH);
      }else{
          Serial.println("Plug1 is Off");
          allItemState[3][1] = state;
          digitalWrite(relay4, LOW);
      }
  }else if(itemnum == "item5"){
      if(state == "on"){
          Serial.println("Plug2 is On");
          allItemState[4][1] = state;
          digitalWrite(relay5, HIGH);
      }else{
          Serial.println("Plug2 is Off");
          allItemState[4][1] = state;
          digitalWrite(relay5, LOW);
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
          
          broadcastRGBvalue(RGBvalues, 3);
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
          // Serial.print("Item number: ");
          // Serial.println(itemnum);
          // Serial.print("State: ");
          // Serial.println(state);

          broadcastItemState(itemnum, state);
          itemsOnOff(itemnum, state);

        }else if(array.size()==3){
          Serial.println("RGBcolor received:");
          // String color = array[0][0].as<String>();
          // String colorvalue = array[0][1].as<String>();

          String rgbarray[3][2];

          for(int i = 0; i<3; i++){
            for(int y=0; y<2; y++){
              rgbarray[i][y] = array[i][y].as<String>();
              RGBvalues[i][y] = array[i][y].as<String>();
            }
          }

          broadcastRGBvalue(rgbarray, 3);

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

  pinMode(switch1pin, INPUT_PULLUP);
  pinMode(switch2pin, INPUT_PULLUP);
  pinMode(switch3pin, INPUT_PULLUP);
  pinMode(switch4pin, INPUT_PULLUP);
  pinMode(switch5pin, INPUT_PULLUP);

  // pinMode(switch6pin, INPUT_PULLUP);
  // pinMode(switch7pin, INPUT_PULLUP);

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(relay5,OUTPUT);

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(relay5, LOW);

  // pinMode(relay6,OUTPUT);
  // pinMode(relay7,OUTPUT);
}

// int prvstate = 0;
String swPrevState[][2] = {
  {"switch1", "1"},
  {"switch2", "1"},
  {"switch3", "1"},
  {"switch4", "1"},
  {"switch5", "1"}
};

void loop() {
  // Handle client requests
  server.handleClient();

  // Handle WebSocket events
  webSocket.loop();

  // if (Serial.available() > 0) {
  //   // Read the incoming byte:
  //   int input1 = Serial.parseInt();
  //   delay(100);
  //   Serial.print("Your input value: ");
  //   Serial.println(input1);

  //   if(prvstate != input1){  //Catch event of Switch

    //   if(allItemState[0][1] == "off"){
    //     broadcastItemState("item1","on");
    //     itemsOnOff("item1","on");
    //     prvstate = input1;
    //   }else{
    //     broadcastItemState("item1","off");
    //     itemsOnOff("item1","off");
    //     prvstate = input1;
    //   }
    // }


  // }

  int switch1val = digitalRead(switch1pin);
  int switch2val = digitalRead(switch2pin);
  int switch3val = digitalRead(switch3pin);
  int switch4val = digitalRead(switch4pin);
  int switch5val = digitalRead(switch5pin);

  int switchval[] = {switch1val, switch2val, switch3val, switch4val, switch5val};

for(int i=0; i<5; i++){
  if(swPrevState[i][1] != String(switchval[i])){
    if(allItemState[i][1]=="off"){
      broadcastItemState(allItemState[i][0],"on");
      itemsOnOff(allItemState[i][0],"on");
      swPrevState[i][1] = String(switchval[i]);
    }else{
      broadcastItemState(allItemState[i][0],"off");
      itemsOnOff(allItemState[i][0],"off");
      swPrevState[i][1] = String(switchval[i]);
    }
  }
}

// //physical_switch1
//   if(swPrevState[0][1] != String(switch1val)){
//     if(allItemState[0][1]=="off"){
//       broadcastItemState("item1","on");
//       itemsOnOff("item1","on");
//       swPrevState[0][1] = String(switch1val);
//     }else{
//       broadcastItemState("item1","off");
//       itemsOnOff("item1","off");
//       swPrevState[0][1] = String(switch1val);
//     }
//   }
// //physical_switch2
//   if(swPrevState[1][1] != String(switch2val)){
//     if(allItemState[1][1]=="off"){
//       broadcastItemState("item2","on");
//       itemsOnOff("item2","on");
//       swPrevState[1][1] = String(switch2val);
//     }else{
//       broadcastItemState("item2","off");
//       itemsOnOff("item2","off");
//       swPrevState[1][1] = String(switch2val);
//     }
//   }
// //physical_switch3
//   if(swPrevState[2][1] != String(switch3val)){
//     if(allItemState[2][1]=="off"){
//       broadcastItemState("item3","on");
//       itemsOnOff("item3","on");
//       swPrevState[2][1] = String(switch3val);
//     }else{
//       broadcastItemState("item3","off");
//       itemsOnOff("item3","off");
//       swPrevState[2][1] = String(switch3val);
//     }
//   }
// //physical_switch4
//   if(swPrevState[3][1] != String(switch4val)){
//     if(allItemState[3][1]=="off"){
//       broadcastItemState("item4","on");
//       itemsOnOff("item4","on");
//       swPrevState[3][1] = String(switch4val);
//     }else{
//       broadcastItemState("item4","off");
//       itemsOnOff("item4","off");
//       swPrevState[3][1] = String(switch4val);
//     }
//   }
// //physical_switch5
//   if(swPrevState[4][1] != String(switch5val)){
//     if(allItemState[4][1]=="off"){
//       broadcastItemState("item5","on");
//       itemsOnOff("item5","on");
//       swPrevState[4][1] = String(switch5val);
//     }else{
//       broadcastItemState("item5","off");
//       itemsOnOff("item5","off");
//       swPrevState[4][1] = String(switch5val);
//     }
//   }



//LAST UPDATE
  // TODO: Your code here
}
