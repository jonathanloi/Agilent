#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "Webpages.h"
extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}

//defines the WPA2-Enterprise details
static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";

ESP8266WebServer server(80);
const int led = LED_BUILTIN;

//define global variables
String temp = "";
char arrayTostore[20];


//Wifi connect function to connect to WPA2-Enterprise wifi signals
void wificonnect() {                

  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);

  wifi_station_set_config(&wifi_config);

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));

  wifi_station_connect();
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
void input_num() {                     
  String s = inputnum;
  String _hostname = EEPROM.get(1, arrayTostore);
  server.send(200, "text/html",s);
  if (server.arg(0)!= "" && _hostname != server.arg(0)){
    temp = server.arg(0);
    Serial.println(temp);
    temp.toCharArray(arrayTostore, temp.length()+1);
    EEPROM.put(1, arrayTostore);
    EEPROM.commit();
    Serial.println(EEPROM.get(1, arrayTostore));
    delay(2000);
    ESP.restart();
  }
}


//html for eeprom memory clear
void memClear(){
  String s = mem_clear;
  server.send(200, "text/html",s);
  EEPROM.put(1,0);
  EEPROM.commit();
  }


//html for the error page
void handleNotFound(){
  String s = Error;
  server.send(404, "text/html",s);
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(512);
  String _hostname = EEPROM.get(1, arrayTostore);    //set the string _hostname the stored value in eeprom
  Serial.println(_hostname.length());
  wificonnect();

//if there is no input on the html form, this will run
  if (_hostname == ""){
    if (MDNS.begin("itrolley")) {   //sets the mDNS default name, connect with "itrolley.local"
    Serial.println("1) MDNS responder started: ");    //print when successfully started
    Serial.println("itrolley");
    //Serial.println(host);
  }
  
    server.on("/",input_num); 
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started ");
}

//if there is input it will run these instead
  else {
    if (MDNS.begin(EEPROM.get(1, arrayTostore))) {  //sets the mDNS name, connect with "example.local"
    Serial.println("2) MDNS responder started:");   //print when successfully started
    Serial.println(_hostname);    //print the mDNS name
    }
    server.on("/",input_num);
    server.on("/memclear",memClear); 
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }

 
}

void loop() {
  server.handleClient();
   delay(500);
}
