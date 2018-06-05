#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include "Webpages.h"

//defines the WPA2-Enterprise details
const char* ssid = "abcd";
const char* password = "6oM47!92";

ESP8266WebServer server(80);
const int led = LED_BUILTIN;

//define global variables
String temp = "";
char arrayTostore[20];
String chartt;

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
  delay(7000);
  ESP.restart();
}

//html for menu page
void menu() {
  String s = Menu;
  String ss = s;
  ss.replace("[0, 0, 0, 0, 0, 0, 0]", chartt);
  //Serial.println(s);
  //Serial.println(ss);
  server.send(200, "text/html", ss);
}

//html for the error page
void handleNotFound() {
  String s = Error;
  server.send(404, "text/html", s);
}

String httpGet() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://sgh721qbmq:1337/itrolley?limit=7&sort=id%20DESC"); //Specify request destination

    int httpCode = http.GET(); //Send the request
    String chart = "[";

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "}")
          count++;
      }
      Serial.println(count);
      //Serial.println(payload);             //Print the response payload
      //Serial.println(payload.substring(105,108));
      int testArray[count];
      for (int i = 0; i < count ; i++) {
        String part = getValue(payload, '}', i);
        int dataPos = part.lastIndexOf(":") + 1;
        testArray[i] = part.substring(dataPos).toInt();
        Serial.println(testArray[i]);
        chart += testArray[i];
        chart += ", ";
      }
      chart = chart.substring(0, chart.length() - 2);
      chart += "]";
      Serial.println(chart);
      return chart;
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


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(512);
  String _hostname = EEPROM.get(1, arrayTostore);  //set the string _hostname to the stored value in eeprom
  char* host_name = EEPROM.get(1, arrayTostore);
  Serial.println(_hostname.length());
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
  
  server.handleClient();
  chartt = httpGet();
  
}
