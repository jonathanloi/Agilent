#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


void setup () {
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);
  WiFi.begin("abcd", "6oM47!92");
  
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.println("Connecting...");
    delay(2000);
    
  }
  Serial.println("Connected!");

}
void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://10.179.131.50:1338/itrolley?limit=8&sort=id%20DESC"); //Specify request destination

    int httpCode = http.GET(); //Send the request
    String chart = "[";

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      payload.remove(0,1);
      //Serial.println(payload);             //Print the response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "{")
          count++;
      }
      //Serial.println(count);
      
      //Serial.println(payload.substring(105,108));
      int testArray[count];
      for (int i = count; i > 0 ; i--) {
        String part = "{";
        part += getValue(payload, '{', i);
        Serial.println(part);
        //int dataPos = part.lastIndexOf(":") + 1;
        //testArray[i] = part.substring(dataPos).toInt(); 
        StaticJsonBuffer<200> jBuffer;
        JsonObject& jObject = jBuffer.parseObject(part);
        String tem = jObject["temperature"];
        String tim = jObject["createdAt"];
        testArray[i] = tem.toInt();
        Serial.println(tem);
        Serial.println(tim);
        chart += testArray[i];
        chart += ", ";
      }
      chart = chart.substring(0,chart.length() - 2);
      chart += "]";
      Serial.println(chart);
    } 
    //else Serial.println("An error ocurred");
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

