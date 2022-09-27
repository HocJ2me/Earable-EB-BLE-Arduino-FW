#include <Arduino.h>


#include "config.h"
#include "BLEConnection.h"
#include "ADS1299Driver.h"

BLEConnection bleEsp32 = BLEConnection();
ADS1299 adsEsp32 = ADS1299();
void setup() {
  // Start serial communication 
  Serial.begin(9600);
  bleEsp32.init();
  adsEsp32.init();
  adsEsp32.getDeviceID();

}

void loop() 
{
  bleEsp32.loopDataStream();
}