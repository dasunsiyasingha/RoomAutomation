/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
// #include <iostream.h>

const char* ssid = "dassamobi";     // CHANGE IT
const char* password = "12345678";  // CHANGE IT

WiFiUDP ntpUDP;
//Sri lanka TimeZone is GMT + 5.30
const long TimeZone = (5*60*60) + (30*60); //(5 Hours * 60 Minutes * 60 Seconds) + (30 minutes * 60 seconds) = 19800 seconds 
NTPClient timeClient(ntpUDP, "pool.ntp.org", TimeZone, 60000);

String allItemState[][2] = {
  {"item1", "off"},
  {"item2", "off"},
  {"item3", "off"},
  {"item4", "off"},
  {"item5", "off"},
  {"item6", "off"}
};

String RGBvalues[3] = {"0", "0", "0"};


int redcolorpin = 5; //D1
int greencolorpin = 4; //D2
int bluecolorpin = 0; //D3

int touchsw1 = 16; //D0
int touchsw2 = 14; //D5
int touchsw3 = 12; //D6
int touchsw4 = 13; //D7

unsigned long previousMillis = 0;
unsigned long touchingMillis = 0;
int leastTime = 1000;  


// ESP8266WebServer server(80);
// WebSocketsServer webSocket = WebSocketsServer(81);                        // Web server on port 80

WebSocketsClient webSocket;

unsigned long currentMillis = 0;
unsigned long lastMillis = 0;
uint32_t currentHours = 100; // currentHours is never equal to 100 when functions are correctly working(cant use 0 is 0.0 am when on alarm)
uint32_t currentMinutes = 100; // currentMinutes is never equal to 100 when functions are correctly working(cant use 0 is 0.0 am when on alarm)
// uint32_t currentSeconds = 100;

int Alramfirstminute = 0; //that use for RGB 12pm RED light function continuosly call at 12.00pm minute 


int getHours = 0;
int getMinutes = 0;
int getSeconds = 0;


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  webSocket.begin("192.168.53.107", 81,"/");
  webSocket.onEvent(webSocketEvent);
  
  Serial.print("Wemos Web Client's IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();

  pinMode(redcolorpin, OUTPUT);
  pinMode(greencolorpin, OUTPUT);
  pinMode(bluecolorpin, OUTPUT);


  pinMode(touchsw1, INPUT);
  pinMode(touchsw2, INPUT);
  pinMode(touchsw3, INPUT);
  pinMode(touchsw4, INPUT);

}

void loop() {

  currentMillis = millis();

  webSocket.loop();

  if(WiFi.status() == WL_CONNECTED){ // check Connect or not to WiFi..
    timeClient.update();
      getHours = timeClient.getHours();
      getMinutes = timeClient.getMinutes();
      getSeconds = timeClient.getSeconds();


      currentHours = getHours;
      currentMinutes = getMinutes;
      // Serial.println(currentHours);
      // Serial.println(currentMinutes);
      lastMillis = currentMillis;
    
  }else if(lastMillis == 0 || (currentMillis - lastMillis)/1000 > 3600*2){ // 3600*2 seconds = 2 hours
    Serial.println("Please connect to Wifi to update time..");
  }else{
      currentHours = getHours + static_cast<int>(((currentMillis - lastMillis)/1000)/3600); // 1000 - millis convert to seconds, 3600 - second convert to hours
      currentMinutes = getMinutes + static_cast<int>(((currentMillis - lastMillis)/1000)/60);



      // currentSeconds = getSeconds + static_cast<int>((currentMillis - lastMillis)/1000);
      // Serial.println("Current Hours: ");
      // Serial.println(currentHours);
      // Serial.println("Current Minutes: ");
      // Serial.println(currentMinutes);
  }

  // Serial.println(String(currentHours) + " : " + String(currentMinutes));     //UNCOMMENT FOR DISPLAY TIME

  AlaramSet(12, 0);
  AlaramSet(0, 0);
  



  touchingMillis = millis() - previousMillis;

  int touchsw1val = digitalRead(touchsw1);
  int touchsw2val = digitalRead(touchsw2);
  int touchsw3val = digitalRead(touchsw3);
  int touchsw4val = digitalRead(touchsw4);
  // int touchsw5val = digitalRead(touchsw5);

  int touchswval[] = {touchsw1val, touchsw2val, touchsw3val, touchsw4val};

  // for(int i=0; i<4; i++){
  //     if(touchswval[i] == HIGH && touchingMillis > leastTime){
  //       if(allItemState[i][1] == "off"){
  //         sendSWupdates(allItemState[i][0], "on");
  //       }else{
  //         sendSWupdates(allItemState[i][0], "off");
  //       }

  //       //****
  //       previousMillis = millis();
  //   }
  // }

}

void sendSWupdates(const String item, const String state){
  DynamicJsonDocument doc(1024);

  doc["itemType"] = "switches";

  Serial.println("switches status broadcast: ");
  Serial.println(item);
  Serial.println(state);

  JsonArray array = doc.createNestedArray("valuesArray");
  array.add(item);
  array.add(state);

  String message;
  serializeJson(doc, message);
  Serial.println(message);

  webSocket.sendTXT(message);
}


void webSocketEvent(WStype_t type, uint8_t* payload, size_t length){
  switch (type){
    case WStype_TEXT:
      Serial.printf("WebSocket received : %s\n", payload);
      // webSocket.sendTXT("Hello from wemos mini");

      String message = (char*) payload;

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, message);

      if(!error){
        Serial.print("NO Errors");

        String itemType = doc["itemType"].as<String>();
        JsonArray array = doc["valuesArray"].as<JsonArray>();

        if(itemType == "rgb"){
          Serial.println("RGBcolor received: ");
          for(int i=0; i<3; i++){
            RGBvalues[i] = array[i].as<String>();
          }
          // sendRGBvalue(RGBvalues);
          RGBcolorset(RGBvalues);

        }else if(itemType == "switches"){
          String itemnum = array[0].as<String>();
          String state = array[1].as<String>();
          Serial.println("SW UPDATE received: ");
          if(itemnum == "item1"){
            allItemState[0][0] = itemnum;
            allItemState[0][1] = state;
          
          }else if(itemnum == "item2"){
            allItemState[1][0] = itemnum;
            allItemState[1][1] = state;
            
          }else if(itemnum == "item3"){
            allItemState[2][0] = itemnum;
            allItemState[2][1] = state;
            
          }else if(itemnum == "item4"){
            allItemState[3][0] = itemnum;
            allItemState[3][1] = state;
            
          }else if(itemnum == "item5"){
            allItemState[4][0] = itemnum;
            allItemState[4][1] = state;
            
          }
          
        }

      }else{
        Serial.println("Failed to parse JSON");
      }
      break;
      
  }

}

void RGBcolorset(const String RGBvalues[3]){
  // int reversevalueR = 255 - RGBvalues[0].toInt();
  // int reversevalueG = 255 - RGBvalues[1].toInt();
  // int reversevalueB = 255 - RGBvalues[2].toInt();

  analogWrite(redcolorpin, RGBvalues[0].toInt());
  analogWrite(greencolorpin, RGBvalues[1].toInt());
  analogWrite(bluecolorpin, RGBvalues[2].toInt());
  Serial.println("RED: " + RGBvalues[0]);
  Serial.println("GREEN: " + RGBvalues[1]);
  Serial.println("BLUE: " + RGBvalues[2]);
}


void AlaramSet(const int hour, const int minute){
  if(Alramfirstminute != 1){
    if(currentHours == hour && currentMinutes == minute){
      Serial.println("12PM RGB RED light ON");
      RGBlightPatternAlram(RGBvalues);
    }
  }

  if(currentHours != hour && currentMinutes != minute){
    Alramfirstminute = 0;
  }
}

void RGBlightPatternAlram(const String RGBvalues[3]){
  analogWrite(redcolorpin, 255);
  analogWrite(greencolorpin, 0);
  analogWrite(bluecolorpin, 0);
  delay(200);
  analogWrite(redcolorpin, 0);
  analogWrite(greencolorpin, 0);
  analogWrite(bluecolorpin, 0);
  delay(200);
  analogWrite(redcolorpin, 255);
  analogWrite(greencolorpin, 0);
  analogWrite(bluecolorpin, 0);
  delay(500);
  analogWrite(redcolorpin, 0);
  analogWrite(greencolorpin, 0);
  analogWrite(bluecolorpin, 0);
  delay(200);
  for(int i = 5; i<=255; i+10){
    analogWrite(redcolorpin, i);
    delay(50);
  }
  for(int i = 5; i<=255; i+10){
    analogWrite(bluecolorpin, i);
    delay(50);
  }
   delay(500);
  analogWrite(redcolorpin, 0);
  analogWrite(greencolorpin, 0);
  analogWrite(bluecolorpin, 0);
  delay(200);
  analogWrite(redcolorpin, RGBvalues[0].toInt());
  analogWrite(greencolorpin, RGBvalues[1].toInt());
  analogWrite(bluecolorpin, RGBvalues[2].toInt());

  Alramfirstminute = 1;
}


