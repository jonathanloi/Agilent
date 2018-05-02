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

ESP8266WebServer server(80);
const int led = LED_BUILTIN;
static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";
String temp = "";
char arrayTostore[20];

void wificonnect() {                //Wifi connect function to connect to WPA2-Enterprise wifi signals

  //WiFi.hostname(host);
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
  //Serial.println("Hostname: ");
  //Serial.println(WiFi.hostname());
}

/*void handleRoot() {
  String s = MAIN_page;
  server.send(200, "text/html",s);
}*/

void input_num() {                     //contains the html data for the main input page
  String s = inputnum;
  server.send(200, "text/html",s);
 // Serial.println(EEPROM.get(15, arrayTostore));
  if (server.arg(0)!= ""){
    temp = server.arg(0);
    Serial.println(temp);
    temp.toCharArray(arrayTostore, temp.length()+1);
    EEPROM.put(1, arrayTostore);
    EEPROM.commit();
    Serial.println(EEPROM.get(1, arrayTostore));
  }
}
/*void update_num(){
  int condition = 0;
  String s = numupdate;
    while(condition == 0){
    host = temp;
    Serial.println(host);
    condition ++;
    }
    server.send(200, "text/html", s);
    
 
}*/
void handleNotFound(){
  String s = Error;
  server.send(404, "text/html",s);
}



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(512);
  String _hostname = EEPROM.get(1, arrayTostore);
  Serial.println(_hostname.length());
  wificonnect();

  if (_hostname == ""){
    if (MDNS.begin("itrolley")) {                     //sets the mDNS name, connect with "name.local"
    Serial.println("1) MDNS responder started: ");      //print when successfully started
    Serial.println("itrolley");
    //Serial.println(host);
  }
  
    server.on("/",input_num); 
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started ");
}
  else {
    
    //temp.toCharArray(_hostname,12);
    if (MDNS.begin(EEPROM.get(1, arrayTostore))) {    //sets the mDNS name, connect with "example.local"
    Serial.println("2) MDNS responder started:");       //print when successfully started
    Serial.println(EEPROM.get(1, arrayTostore));
    //Serial.println(_hostname);
    }
    server.on("/",input_num); 
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
  }

  delay(500);
}

void loop() {
  server.handleClient();
}
