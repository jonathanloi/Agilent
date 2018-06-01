#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

 
void setup () {
 
  Serial.begin(115200);
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

    http.begin("http://sgh721qbmq:1337/itrolley/87"); //Specify request destination
 
    int httpCode = http.GET(); //Send the request
 
    if (httpCode > 0) { //Check the returning code
     String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
      StaticJsonBuffer<300> jBuffer;
      JsonObject& jObject = jBuffer.parseObject(payload);
      String nam = jObject["name"];
      String tem = jObject["temperature"];
      Serial.println(nam);
      Serial.println(tem.toInt());
    }else Serial.println("An error ocurred");
 
   http.end();   //Close connection
 
  }

  delay(10000);    //Send a request every 10 seconds
 
}
 
      
