#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <RBD_Timer.h>
#include "EmonLib.h"
#include "Webpages.h"


//defines the WiFi details
const char* ssid = "abcd";
const char* password = "6oM47!92";

ESP8266WebServer server(80);
RBD::Timer temptimer(60000*30); //30 minute timer
RBD::Timer timer1(60000); //1 minute timer
OneWire oneWire(14); //set the temperature read pin to 12
DallasTemperature sensors(&oneWire);
EnergyMonitor emon1;

//define global variables
String temp = "";
char arrayTostore[20];
String tempData, timeData;
bool Presence = 0;
const int trigPin = 2;
const int echoPin = 16;
long duration;
int distance;

//Wifi connect function to connect to WPA2-Enterprise wifi signals
void wificonnect() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// html data for the main input page
void configure_name() {
  String s = configureName;
  String _hostname = EEPROM.get(1, arrayTostore);
  server.send(200, "text/html", s);
  if (server.arg(0) != "" && _hostname != server.arg(0)) {
    temp = server.arg(0);
    Serial.println(temp);
    temp.toCharArray(arrayTostore, temp.length() + 1);
    EEPROM.put(1, arrayTostore);
    EEPROM.commit();
    Serial.println(EEPROM.get(1, arrayTostore));
    delay(2000);
    ESP.restart();
  }
}

//html for eeprom memory clear
void memClear() {
  String s = mem_clear;
  server.send(200, "text/html", s);
  EEPROM.put(1, 0);
  EEPROM.commit();
  Serial.println("EEPROM memory has been reset");
  ESP.restart();
}

//html for menu page
void menu() {
  String s = Menu;
  String ss = s;
  sensors.requestTemperatures(); 
  String temperature = String(sensors.getTempCByIndex(0));
  ss.replace("22",temperature);
  httpDataGet(0);
  ss.replace("[0]", tempData);
  ss.replace("[10]", timeData);
  bool presenceStatus = checkPresence();
  Serial.println(presenceStatus);
    if(presenceStatus == 1){
      ss.replace("FALZ", "YES");
    }
    else{
      ss.replace("FALZ", "NO");
    }
    Serial.println(emon1.calcIrms(1480));
  String rms = String(emon1.calcIrms(1480));
  ss.replace("155", rms);
  //Serial.println(s);
  //Serial.println(ss);
  server.send(200, "text/html", ss);
}

void viewTemp(){ //webpage for showing temperature only graph
  String s = Temperature;
  String ss = s;
  httpDataGet(1);
  ss.replace("[0]", tempData);
  ss.replace("[10]", timeData);
  Serial.println(ss);
  server.send(200, "text/html", ss);
}

//html for the error page
void handleNotFound() {
  String s = Error;
  server.send(404, "text/html", s);
}


void httpDataGet(int type) {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    if(type == 0){
      http.begin("http://10.179.131.50:1338/itrolley?limit=7&sort=id%20DESC"); //Specify request destination
    }
    else if(type == 1){
      http.begin("http://10.179.131.50:1338/itrolley?limit=50&sort=id%20DESC"); //Specify request destination  
    }
    
    int httpCode = http.GET(); //Send the request
    timeData = "['";
    tempData = "[";

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "{")
          count++;
      }
      
      //Serial.println(count);
      //Serial.println(payload);             //Print the response payload
      for (int i = count ; i > 0 ; i--) {
        String part = "{";
        part += getValue(payload, '{', i);
        //Serial.println(part);
        StaticJsonBuffer<500> jBuffer;
        JsonObject& jObject = jBuffer.parseObject(part);
        
        String tem = jObject["temperature"];
        String tim = jObject["createdAt"];
        
        //Serial.println(tem);
        //Serial.println(tim);
        int utime = tim.substring(0,tim.length() - 3).toInt(); //removes the milliseconds
        time_t t = utime + 28800; //converts to local time
        timeData += day(t);
        timeData += "/";
        timeData += month(t);
        timeData += "/";
        timeData += year(t);
        timeData += " ";
        timeData += hour(t);
        timeData += ":";
        int minut = minute(t);
          if (minut < 10){ // add a zero before the minute value if its single digit
            timeData += "0";
            timeData += minute(t);
          }
          else{
            timeData += minut;
          }
        timeData += "','";
        tempData += tem;
        tempData += ", ";
      }
      timeData = timeData.substring(0, timeData.length() - 2); //remove additional 
      timeData += "]"; //close string
      //Serial.println(timeData);
      tempData = tempData.substring(0, tempData.length() - 2);
      tempData += "]";
      //Serial.println(tempData);
    }
    else Serial.println("An error ocurred");
    http.end();   //Close connection

  }
  //delay(10000);    //Send a request every 10 seconds
}


String getValue(String data, char separator, int index) {

  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void postTempValue(float temperature) {
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["name"] = "Temperature";
    JSONencoder["temperature"] = temperature;
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    //Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://10.179.131.50:1338/itrolley");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload

    //Serial.println(httpCode);   //Print HTTP return code
    //Serial.println(payload);    //Print request response payload 
 
    http.end();  //Close connection
    }
    else {
    Serial.println("Error in WiFi connection");
  }
 
  //delay(10000);  //Send a request every 30 seconds
 
}

void UltrasonicSensor(){
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
}

bool checkPresence(){
  
  UltrasonicSensor();
  bool metalDetector = digitalRead(12);
  bool presence;

  if( metalDetector == 1 && distance <=10){
    presence = 1;
  }
  else {
    presence = 0;
  }
  return presence;
}

void postPresence(){
  
  bool presence = checkPresence();
  if(Presence != presence){ //only post new value if there is a difference
    
    Presence = presence; //set the global variable to local value
  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 

    JSONencoder["name"] = "~~" ;
    JSONencoder["instrumentPresence"] = presence;
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    //Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://wpsgpwbofs1:1338/presence");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload

    //Serial.println(httpCode);   //Print HTTP return code
    //Serial.println(payload);    //Print request response payload 
 
    http.end();  //Close connection
    }
   else {
    Serial.println("Error in WiFi connection");
   }
 }  
 else{}
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(12,INPUT);
  emon1.current(17, 3.137);             // Current: input pin, calibration.
  EEPROM.begin(512);
  sensors.begin();
  String _hostname = EEPROM.get(1, arrayTostore);  //set the string _hostname to the stored value in eeprom
  char* host_name = EEPROM.get(1, arrayTostore);
  temptimer.restart();
  timer1.restart();
  
  //Serial.println(_hostname.length());
  wificonnect();
  //if there is no input on the html form, this will run
  if (_hostname == "" || _hostname == "itrolley") {
    if (MDNS.begin("itrolley")) {   //sets the mDNS default name, connect with "itrolley.local"
      Serial.println("1) MDNS responder started: ");    //print when successfully started
      Serial.println("itrolley");
    }

    server.on("/", menu);
    server.on("/configure", configure_name);
    server.on("/memclear", memClear);
    server.on("/temperature", viewTemp);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started ");
  }

  //if there is input it will run these instead
  else {
    if (MDNS.begin(host_name)) {  //sets the mDNS name, connect with "example.local"
      Serial.println("2) MDNS responder started:");   //print when successfully started
      Serial.println(host_name);    //print the mDNS name
    }

    server.on("/", menu);
    server.on("/configure", configure_name);
    server.on("/memclear", memClear);
    server.on("/temperature", viewTemp);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }

  sensors.requestTemperatures(); 
  postTempValue(sensors.getTempCByIndex(0));
  
}

void loop() {

  if (WiFi.status() != WL_CONNECTED){
      wificonnect();
  }
  if (temptimer.onRestart()){
      sensors.requestTemperatures(); 
      postTempValue(sensors.getTempCByIndex(0));
  }
  if (timer1.onRestart()){
    postPresence();
    Serial.println("Posting...");
  }
  server.handleClient();
  
}


