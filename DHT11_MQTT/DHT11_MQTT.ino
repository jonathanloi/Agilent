#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include "DHT.h"
#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>

#define WIFI_SSID "5CG6182HKM 0819"
#define WIFI_PASS "6oM47!92"
#define MQTT_SERVER "192.168.137.1"
#define DHTPIN 13     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT11 

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);
DHT dht(DHTPIN, DHTTYPE);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

int value = 0;
String s = "";

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print(s+"Connecting to WiFi: "+WIFI_SSID+" ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(s+" connected with IP: "+WiFi.localIP());

  // Connect to MQTT
  Serial.print(s+"Connecting to MQTT: "+MQTT_SERVER+" ... ");
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");

    mqtt.subscribe("test/inTopic1", topic1_subscriber);
    mqtt.subscribe("test/inTopic2", topic2_subscriber);
  } else {
    Serial.println(s+"failed, rc="+client.state());
  }

  // Enable Thread
  thread.onRun(publisher);
  thread.setInterval(2000);
  threadControl.add(&thread);
}

void loop() {
  client.loop();
  threadControl.run();
  Serial.println("HElloo");
}

void publisher() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t) ) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Temperature: ");
  Serial.print(t);
  ++value;
  mqtt.publish("test/outTopic", s+"Humidity "+h +s+"\nTemperature: "+t);
}
void topic1_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
}
void topic2_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 2 ["+topic+"] "+message);
}
