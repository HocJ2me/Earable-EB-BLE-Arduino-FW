#include <Arduino.h>

#include "config.h"
#include "BLEConnection.h"
#include "ADS1299Driver.h"

BLEConnection bleEsp32 = BLEConnection();
ADS1299 ads1299 = ADS1299();
void setup() {
  // Start serial communication 
  Serial.begin(9600);
  bleEsp32.init();
  ads1299.init();
  ads1299.getDeviceID();

}
boolean deviceIDReturned = false;
boolean startedLogging = false;
void loop() 
{
  
  if(deviceIDReturned == false){
    
    ads1299.getDeviceID(); //Funciton to return Device ID
    
    //prints dashed line to separate serial print sections
    Serial.println("----------------------------------------------");
    
    //Read ADS1299 Register at address 0x00 (see Datasheet pg. 35 for more info on SPI commands)
    ads1299.RREG(0x00);
    Serial.println("----------------------------------------------");
    
    //PRINT ALL REGISTERS... Read 0x17 addresses starting from address 0x00 (these numbers can be replaced by binary or integer values)
    ads1299.RREG(0x00, 0x17);
    Serial.println("----------------------------------------------");
    
    //Write register command (see Datasheet pg. 38 for more info about WREG)
    ads1299.WREG(CONFIG1, 0b11010110);
    Serial.println("----------------------------------------------");
    
    //Repeat PRINT ALL REGISTERS to verify that WREG changed the CONFIG1 register
    ads1299.RREG(0x00, 0x17);
    Serial.println("----------------------------------------------");
    
    //Start data conversions command
    ads1299.start(); //must start before reading data continuous
    deviceIDReturned = true;
  }
  
  //print data to the serial console for only the 1st 10seconds of 
  while(millis()<100000){
    if(startedLogging == false){
      Serial.print("Millis: "); //this is to see at what time the data starts printing to check for timing accuracy (default sample rate is 250 sample/second)
      Serial.println(millis());
      startedLogging = true;
    }
    
    //Print Read Data Continuous (RDATAC) to Ardiuno serial monitor... 
    //The timing of this method is not perfect yet. Some data is getting lost 
    //and I believe its due to the serial monitor taking too much time to print data and not being ready to recieve to packets
    ads1299.updateData();  
  }
  
  }