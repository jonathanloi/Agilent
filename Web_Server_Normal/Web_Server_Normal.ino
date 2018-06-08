#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <RBD_Timer.h>
#include "Webpages.h"

//defines the WPA2-Enterprise details
const char* ssid = "abcd";
const char* password = "6oM47!92";

DHT dht (13, DHT11);
ESP8266WebServer server(80);
RBD::Timer temptimer(60000*10);


//define global variables
String temp = "";
char arrayTostore[20];
String tempData, timeData;


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
  //httpTempGet();
  httpDataGet();
  ss.replace("[0, 0, 0, 0, 0, 0, 0]", tempData);
  delay(10);
  ss.replace("[10,10,10,10,10,10,10]", timeData);
  //Serial.println(s);
  Serial.println(ss);
  server.send(200, "text/html", ss);
}

//html for the error page
void handleNotFound() {
  String s = Error;
  server.send(404, "text/html", s);
}


/*void httpTempGet() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://10.179.131.50:1338/itrolley?limit=7&sort=id%20DESC"); //Specify request destination

    int httpCode = http.GET(); //Send the request
    tempData = "[";

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "}")
          count++;
      }
      //Serial.println(count);
      //Serial.println(payload);             //Print the response payload
      int testArray[count];
      for (int i = 0; i < count ; i++) {
        String part = getValue(payload, '}', i);
        int dataPos = part.lastIndexOf(":") + 1;
        testArray[i] = part.substring(dataPos).toInt();
        //Serial.println(testArray[i]);
        tempData += testArray[i];
        tempData += ", ";
      }
      tempData = tempData.substring(0, tempData.length() - 2);
      tempData += "]";
      //Serial.println(chart);
    }
    else Serial.println("An error ocurred");
    http.end();   //Close connection

  }
  //delay(10000);    //Send a request every 10 seconds
}
*/


void httpDataGet() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://10.179.131.50:1338/itrolley?limit=7&sort=id%20DESC"); //Specify request destination

    int httpCode = http.GET(); //Send the request
    timeData = "['";
    tempData = "[";

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "}")
          count++;
      }
      //Serial.println(count);
      //Serial.println(payload);             //Print the response payload
      int testArray[count];
      int testArray1[count];
      for (int i = count - 1 ; i >= 0 ; i--) {
        String part = getValue(payload, '}', i);
        testArray1[i] = part.substring(22, 33).toInt();
        //Serial.println(testArray[i]);
        time_t t = testArray1[i] + 28800;
        timeData += day(t);
        timeData += "/";
        timeData += month(t);
        timeData += "/";
        timeData += year(t);
        timeData += " ";
        timeData += hour(t);
        timeData += ":";
        int minut = minute(t);
        if (minut < 10){
          timeData += "0";
          timeData += minute(t);
        }
        else{
          timeData += minut;
        }
        timeData += "','";

        int dataPos = part.lastIndexOf(":") + 1;
        testArray[i] = part.substring(dataPos).toInt();
        //Serial.println(testArray[i]);
        tempData += testArray[i];
        tempData += ", ";
      }
      timeData = timeData.substring(0, timeData.length() - 2);
      timeData += "]";
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
    //JsonArray& values = JSONencoder.createNestedArray("temperature"); //JSON array
    //values.add(20); //Add value to array
    //values.add(21); //Add value to array
    //values.add(23); //Add value to array
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    //Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://10.179.131.50:1338/itrolley");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload

    //Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload 
 
    http.end();  //Close connection
    }
    else {
    Serial.println("Error in WiFi connection");
  }
 
  //delay(10000);  //Send a request every 30 seconds
 
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(512);
  String _hostname = EEPROM.get(1, arrayTostore);  //set the string _hostname to the stored value in eeprom
  char* host_name = EEPROM.get(1, arrayTostore);
  temptimer.restart();
  
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
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }

}

void loop() {

  if (temptimer.onRestart()){
  postTempValue(dht.readTemperature());
  }
  server.handleClient();
  
}


