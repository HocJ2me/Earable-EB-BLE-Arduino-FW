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
void ADS1299::start()
{
  spiCommand(_START);
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
  outputCount = 0;
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


void ADS1299::RREG(byte _address) {
    byte opcode1 = _RREG + _address; //001rrrrr; _RREG = 00100000 and _address = rrrrr
    SPI_csSetLow();
    SPI_xfer(_SDATAC); //SDATAC
    SPI_xfer(opcode1); //RREG
    SPI_xfer(0x00); //opcode2
    byte data = SPI_xfer(0x00); // returned byte should match default of register map unless edited manually (Datasheet, pg.39)
    Serial.print("0x");
    if(_address<16) Serial.print("0");
    Serial.print(_address, HEX);
    Serial.print(", ");
    Serial.print("0x");
    if(data<16) Serial.print("0");
    Serial.print(data, HEX);
    Serial.print(", ");
    for(byte j = 0; j<8; j++){
        Serial.print(bitRead(data, 7-j), BIN);
        if(j!=7) Serial.print(", ");
    }
    SPI_xfer(_RDATAC); //turn read data continuous back on
    SPI_csSetHigh();
    Serial.println();
}

void ADS1299::RREG(byte _address, byte _numRegistersMinusOne) {
    byte opcode1 = _RREG + _address; //001rrrrr; _RREG = 00100000 and _address = rrrrr
    SPI_csSetLow();
    SPI_xfer(_SDATAC); //SDATAC
    SPI_xfer(opcode1); //RREG
    SPI_xfer(_numRegistersMinusOne); //opcode2
    for(byte i = 0; i <= _numRegistersMinusOne; i++){
        byte data = SPI_xfer(0x00); // returned byte should match default of register map unless previously edited manually (Datasheet, pg.39)
        Serial.print("0x");
        if(i<16) Serial.print("0"); //lead with 0 if value is between 0x00-0x0F to ensure 2 digit format
        Serial.print(i, HEX);
        Serial.print(", ");
        Serial.print("0x");
        if(data<16) Serial.print("0"); //lead with 0 if value is between 0x00-0x0F to ensure 2 digit format
        Serial.print(data, HEX);
        Serial.print(", ");
        for(byte j = 0; j<8; j++){
            Serial.print(bitRead(data, 7-j), BIN);
            if(j!=7) Serial.print(", ");
        }
        Serial.println();
    }
    SPI_xfer(_RDATAC); //turn read data continuous back on
    SPI_csSetHigh();
}


void ADS1299::WREG(byte _address, byte _value) {
    byte opcode1 = _WREG + _address; //001rrrrr; _RREG = 00100000 and _address = rrrrr
    SPI_csSetLow();
    SPI_xfer(_SDATAC); //SDATAC
    SPI_xfer(opcode1);
    SPI_xfer(0x00);
    SPI_xfer(_value);
    SPI_xfer(_RDATAC);
    SPI_csSetHigh();
    Serial.print("Register 0x");
    Serial.print(_address, HEX);
    Serial.println(" modified.");
}

void ADS1299::getDeviceID() 
{
  // byte _byteName =   RREG(ID);
  Serial.print("ADS_ID: ");
  RREG(ID);
  // Serial.println(_byteName, HEX);
}

void ADS1299::updateData(){
    if(digitalRead(GPIO_DRDY) == LOW){
        SPI_csSetLow();
//        long output[100][9];
        long output[7]; // 6 channel and 1 is status bit
        long dataPacket;
        for(int i = 0; i<7; i++){
            for(int j = 0; j<3; j++){
                byte dataByte =  SPI_xfer(0x00);
                dataPacket = (dataPacket<<8) | dataByte;
            }
//            output[outputCount][i] = dataPacket;
            output[i] = dataPacket;
            dataPacket = 0;
        }
        SPI_csSetHigh();
        Serial.print(outputCount);
        Serial.print(", ");
        for (int i=0;i<9; i++) {
            Serial.print(output[i], HEX);
            if(i!=8) Serial.print(", ");
            
        }
        Serial.println();
        outputCount++;
    }
}


ADS1299::ADS1299()
{
}

ADS1299::~ADS1299()
{
}