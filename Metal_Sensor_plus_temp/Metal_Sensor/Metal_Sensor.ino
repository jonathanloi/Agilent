#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(14);
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(12,INPUT); 
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool qwe = digitalRead(12);
  Serial.println(qwe);
  sensors.requestTemperatures(); 
  Serial.println(sensors.getTempCByIndex(0));
  delay(1000);
}
