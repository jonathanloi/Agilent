#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";

void setup () {

  Serial.begin(115200);
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

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");

  }
  Serial.println("Connected to WiFi Network");

}
void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://sgh721qbmq:1337/itrolley?limit=10"); //Specify request destination

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
        int test = part.lastIndexOf(":") + 1;
        testArray[i] = part.substring(test).toInt();
        Serial.println(testArray[i]);
        chart += testArray[i];
        chart += ", ";
      }
      chart = chart.substring(0,chart.length() - 2);
      chart += "]";
      Serial.println(chart);
      /*Serial.println(part1);
        Serial.println(part2);
        Serial.println(part3);
        Serial.println(part4);*/
      //Serial.println(part1);
      //int test = part1.lastIndexOf(":") + 1;
      //Serial.println(part1.substring(test));
      //Serial.println(part2);
      /*StaticJsonBuffer<300> jBuffer;
        JsonObject& jObject = jBuffer.parseObject(payload);
        String nam = jObject["name"][0];
        String tem = jObject["temperature"][0];
        Serial.println(nam);
        Serial.println(tem.toInt());*/
    } else Serial.println("An error ocurred");

    http.end();   //Close connection

  }

  delay(10000);    //Send a request every 10 seconds

}


String getValue(String data, char separator, int index)
{
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

