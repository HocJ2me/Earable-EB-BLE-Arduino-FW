//check pakage
//check sample rate lose
// check decode
#include <Arduino.h>

#include "config.h"
#include "BLEConnection.h"
#include "ADS1299Driver.h"

BLEConnection bleEsp32 = BLEConnection();
ADS1299 ads1299 = ADS1299();
volatile bool dataReady = false;

void IRAM_ATTR isr()
{
  dataReady = true;
}

void setup() {
  // Start serial communication 
  Serial.begin(9600);
  bleEsp32.init();
  ads1299.init();
  ads1299.getDeviceID();
  attachInterrupt(digitalPinToInterrupt(27), isr, FALLING);

}
boolean deviceIDReturned = false;
boolean startedLogging = false;
unsigned long numberOfSample = 0;
unsigned long lastNumberOfSample = 0;
unsigned long lastPrintNumberOfSample = 0;
void loop() 
{
  
  if(deviceIDReturned == false)
  {
    //Funciton to return Device ID
    ads1299.getDeviceID(); 
    
    //prints dashed line to separate serial print sections
    Serial.println("----------------------------------------------");
    
    //STOP STREAMING DATA
    ads1299.spiCommand(_SDATAC);
    
    //Read ADS1299 Register at address 0x00 (see Datasheet pg. 35 for more info on SPI commands)
    ads1299.RREG(0x00);
    Serial.println("----------------------------------------------");
    
    //PRINT ALL REGISTERS... Read 0x17 addresses starting from address 0x00 (these numbers can be replaced by binary or integer values)
    ads1299.RREG(0x00, 0x17);
    Serial.println("----------------------------------------------");
    
    /* Configure sampling rate */
    ads1299.WREG(CONFIG1, 0b10010110);
    Serial.println("----------------------------------------------");

    /* Configure clock and amplitude*/
    ads1299.WREG(CONFIG2, 0b11000000);
    Serial.println("----------------------------------------------");

    /* Configure default gain */
    for (uint8_t i = 0; i < 4; i++)
    {
        ads1299.activeChannel(i+1, false);
        ads1299.setGain(i+1, AFE_DEFAULT_CONFIG_GAIN);
    }
    /* Configure bias*/
    ads1299.WREG(CONFIG3, 0b11111000);
    Serial.println("----------------------------------------------");

    /* Configure lead off */
    ads1299.WREG(LOFF, (0x00|0x03));
    Serial.println("----------------------------------------------");
    
    /* Connect all N channels together*/
    ads1299.WREG(MISC1, 0x20);
    Serial.println("----------------------------------------------");

    /* Config channel 3 and 4 in normal operation mode (impedance measurement off)*/
    for (uint8_t i = 0; i < 4; i++)
    {
        ads1299.leadOffSet(i+1, false, false);
        ads1299.activeChannel(i+1, true);
    }

    //Repeat PRINT ALL REGISTERS to verify that WREG changed the  registers
    ads1299.RREG(0x00, 0x17);
    Serial.println("----------------------------------------------");
    
    //Contunue Streamming
    ads1299.spiCommand(_RDATAC);

    //Start data conversions command
    ads1299.start(); //must start before reading data continuous
    deviceIDReturned = true;
  }
  
  //print data to the serial console for only the 1st 10seconds of 
  while(millis()< 5 * 60 * 1000)
  {
    if(startedLogging == false){
      Serial.print("Millis: "); //this is to see at what time the data starts printing to check for timing accuracy (default sample rate is 250 sample/second)
      Serial.println(millis());
      startedLogging = true;
    }
    
    //Print Read Data Continuous (RDATAC) to Ardiuno serial monitor... 
    //The timing of this method is not perfect yet. Some data is getting lost 
    //and I believe its due to the serial monitor taking too much time to print data and not being ready to recieve to packets
    if(dataReady == true)
    {
      dataReady = false;
      if(ads1299.updateData())
      {
        numberOfSample ++;
        if(numberOfSample % 9 == 0)
        {
          long **rawAdsData = ads1299.dataExport();
          bleEsp32.loopDataStream(rawAdsData);
        //   // Serial.println("Raw data:");
        //   // for(int i=0;i<5;i++)
        //   // {
        //   //   Serial.print(rawAdsData[0][i], HEX);
        //   //   Serial.print(",");
        //   // }
        //   // Serial.println();
        }
      }  
    }
    // if(millis() - lastPrintNumberOfSample  > 1000)
    // {
    //     Serial.println(numberOfSample - lastNumberOfSample);
    //     lastNumberOfSample = numberOfSample;
    //     lastPrintNumberOfSample = millis();
    // }
  }
  
}