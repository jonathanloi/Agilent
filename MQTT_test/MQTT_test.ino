#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}


/************************* WiFi Access Point *********************************/

static const char* ssid = "spark";
static const char* username = "jonatloi";
static const char* password = "Jljon1999!";


/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  8883                   // use 8883 for SSL
#define AIO_USERNAME    "jonathanloi"
#define AIO_KEY         "bc460b1606a44605b83a16a38ba5bc6e"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");


// Setup a feed called 'onoff' for subscribing to changes to the button
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/toggle",MQTT_QOS_1);

/*************************** Sketch Code ************************************/


void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the button value is: ");
  Serial.println(data);

     String message = String(data);
      message.trim();
      if (message == "ON") {digitalWrite(LED_BUILTIN, HIGH);}
      if (message == "OFF") {digitalWrite(LED_BUILTIN, LOW);}
  
}


void setup() {

    pinMode(LED_BUILTIN, OUTPUT);

    
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
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


  onoffbutton.setCallback(onoffcallback);

  mqtt.subscribe(&onoffbutton);

}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();





  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(500);
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  
 // if(! mqtt.ping()) {
 //   mqtt.disconnect();
 // }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
