#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>

static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";
#define MQTT_SERVER "10.56.70.100"
extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}
WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

ThreadController threadControl = ThreadController();
Thread thread = Thread();

int value = 0;
String s = "";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  // Connect to WiFi
  delay(10);
  // We start by connecting to a WiFi network
  wifi_set_opmode(STATION_MODE);
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  wifi_station_set_config(&wifi_config);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));
  wifi_station_connect();
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
  // Serial.println(WiFi.RSSI());
  // Connect to MQTT
  Serial.print(s + "Connecting to MQTT: " + MQTT_SERVER + " ... ");
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");
    mqtt.subscribe("test/inTopic1", topic1_subscriber);
    mqtt.subscribe("test/inTopic2", topic2_subscriber);
  }
  else {
    Serial.println(s + "failed, rc=" + client.state());
  }

  // Enable Thread
  thread.onRun(publisher);
  thread.setInterval(2000);
  threadControl.add(&thread);
}

void loop() {
  client.loop();
  threadControl.run();
}

void publisher() {
  ++value;
  mqtt.publish("test/outTopic", s + "hello world " + value);
}
void topic1_subscriber(String topic, String message) {
  Serial.println(s + "Message arrived in function 1 [" + topic + "] " + message);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}
void topic2_subscriber(String topic, String message) {
  Serial.println(s + "Message arrived in function 2 [" + topic + "] " + message);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
}
