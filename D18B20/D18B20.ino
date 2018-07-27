#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(14);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(115200);
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); 
  Serial.println(sensors.getTempCByIndex(0));
  delay(1000);
}
