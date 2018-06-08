
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "abcd";
const char* password = "6oM47!92";

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
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.begin(115200);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://10.179.131.50:1338/itrolley?limit=7&sort=id%20DESC"); //Specify request destination

    int httpCode = http.GET(); //Send the request
    String chart = "['";
    //Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      int count = 0;
      for (int i = 0; i <= payload.length(); i++) {
        if (payload.substring(i, i + 1) == "}")
          count++;
      }
      //Serial.println(count);
      Serial.println(payload);             //Print the response payload
      int testArray[count];
      for (int i = count - 1; i >= 0 ; i--) {
        String part = getValue(payload, '}', i);
        testArray[i] = part.substring(22, 33).toInt();
        //Serial.println(testArray[i]);
        time_t t = testArray[i] + 28800;
        chart += day(t);
        chart += "/";
        chart += month(t);
        chart += "/";
        chart += year(t);
        chart += " ";
        chart += hour(t);
        chart += ":";
        chart += minute(t);
        chart += "','";
      }
      chart = chart.substring(0, chart.length() - 2);
      chart += "]";
      Serial.println(chart);

    }
    else Serial.println("An error ocurred");
    http.end();   //Close connection

  }
  delay(10000);    //Send a request every 10 seconds
}

