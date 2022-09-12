#include <Arduino.h>


#include "config.h"
#include "BLEConnection.h"


// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

BLEConnection BLE_ESP;

void setup() {
  // Start serial communication 
  Serial.begin(9600);
  BLE_ESP.init();
}

void loop() 
{
  
}