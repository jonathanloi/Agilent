#include <ESP8266Ping.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";
const char* remote_host = "sgh721qbmq";

void setup() {
 
  Serial.begin(115200);                            //Serial connection
  /*wifi_set_opmode(STATION_MODE);
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  wifi_station_set_config(&wifi_config);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);
  //wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));
  wifi_station_connect();*/
  WiFi.begin("abcd", "6oM47!92");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  

  Serial.print("Pinging host ");
  Serial.println(remote_host);

  if(Ping.ping(remote_host)) {
    Serial.println("Success!!");
  } else {
    Serial.println("Error :(");
  }
 
}
 
void loop() {
  char test = 53;
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["name"] = "Temperature";
    JSONencoder["temperature"] = "40";
    //JsonArray& values = JSONencoder.createNestedArray("temperature"); //JSON array
    //values.add(20); //Add value to array
    //values.add(21); //Add value to array
    //values.add(23); //Add value to array
 
    /*JsonArray& timestamps = JSONencoder.createNestedArray("timestamps"); //JSON array
    timestamps.add("10:10"); //Add value to array
    timestamps.add("10:20"); //Add value to array
    timestamps.add("10:30"); //Add value to array*/
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://sgh721qbmq:1337/itrolley");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload

   
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload 
 
    http.end();  //Close connection

    Serial.print("Pinging host ");
    Serial.println(remote_host);
    if(Ping.ping(remote_host)) {
    Serial.println("Success!!");
  } else {
    Serial.println("Error :(");
  }
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(10000);  //Send a request every 30 seconds
 
}
