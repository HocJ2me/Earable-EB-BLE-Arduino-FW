#include "ADS1299Driver.h"

byte ADS1299::spiCommand(byte data)
{
  SPI_csSetLow();
  byte res = SPI_xfer(data);
  SPI_csSetHigh();
  return res;
}
void ADS1299::SPI_csSetLow()
{
  digitalWrite(vspi->pinSS(), LOW); //pull SS slow to prep other end for transfer
}
void ADS1299::SPI_csSetHigh()
{
  digitalWrite(vspi->pinSS(), HIGH); //pull SS slow to prep other end for transfer
}
byte ADS1299::SPI_xfer(byte _data) {
  vspi->beginTransaction(SPISettings(CLK_SPI, MSBFIRST, SPI_MODE1));
  byte res = vspi->transfer(_data);
  vspi->endTransaction();
  return res;
}
void ADS1299::reset()
{
  spiCommand(_RESET); 
  delay(10);  //delay to ensure connection
  spiCommand(_SDATAC); 
  delay(10);  //delay to ensure connection
}
void ADS1299::init()
{
  vspi = new SPIClass(VSPI);  
  vspi->begin(GPIO_SCLK, GPIO_MISO, GPIO_MOSI, GPIO_CS); //SCLK, MISO, MOSI, SS
  pinMode(vspi->pinSS(), OUTPUT); //VSPI SS
  // initalize the  data ready and chip select pins:
  pinMode(GPIO_DRDY, INPUT_PULLUP);
  pinMode(GPIO_CS, OUTPUT);
  reset();
}


void ADS1299::logByte2Serial(byte _byte, byte _address)
{
    Serial.print("0x");
    if(_address<16) Serial.print("0");
    Serial.print(_address, HEX);
    Serial.print(", ");
    Serial.print("0x");
    if(_byte<16) Serial.print("0");
    Serial.print(_byte, HEX);
    Serial.print(", ");
    for(byte j = 0; j<8; j++){
        Serial.print(bitRead(_byte, 7-j), BIN);
        if(j!=7) Serial.print(", ");
    }
    Serial.println();
}

byte ADS1299::RREG(byte _address) {

    byte opcode1 = _RREG + _address; //001rrrrr; _RREG = 00100000 and _address = rrrrr
    SPI_csSetLow();
    SPI_xfer(opcode1); //RREG
    SPI_xfer(0x00); //opcode2
    byte data = SPI_xfer(0x00); // returned byte should match default of register map unless edited manually (Datasheet, pg.39)
    SPI_csSetHigh();
    return data;
}

void ADS1299::getDeviceID() 
{
  byte _byteName =   RREG(ADS_ID);
  Serial.print("ADS_ID: ");
  Serial.println(_byteName, HEX);
}


ADS1299::ADS1299()
{
}

ADS1299::~ADS1299()
{
}