#include "EmonLib.h"
// Include Emon Library
EnergyMonitor emon1;
// Create an instance
void setup()
{
  Serial.begin(115200);

  emon1.current(17, 3.0716);             // Current: input pin, calibration.
}

void loop()
{
double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  Serial.println(Irms);             // Irms
}
