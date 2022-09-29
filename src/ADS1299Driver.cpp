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
  
  outputData = new long *[10]; 
  for(int i = 0; i <10; i++) 
    outputData[i] = new long[5];
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
    return data;
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
        long output[5] = {0}; // 4 channel and 1 is status bit, 5*3 = 15 byte
        long dataPacket = 0;    // data Packet for storage
        byte dataFirstByte; // to check bit
        for(int i = 0; i<5; i++)    // read 5 * 3 = 15 byte
        {
            for(int j = 0; j<3; j++)  // read 3 byte each channel and status
            {
                byte dataByte =  SPI_xfer(0x00);
                dataPacket = (dataPacket<<8) | dataByte;    // bit left shift
                if(j == 0)
                {
                    dataFirstByte = dataByte;               // storage first byte
                }
            }
            if(i > 0)
            {
                if(dataFirstByte & 0x80)                        // check first byte
                {
                    output[i] |= 0xFF000000;
                }
                output[i] = long(dataPacket * 0.08940697249); // convert channel (using code in ads1299_test.c)
            }
            else
            {
                output[i] = dataPacket;
            }
            outputData[outputCount][i] = output[i];
            dataPacket = 0;
        }
        SPI_csSetHigh();
        Serial.print(outputCount); 
        Serial.print(", ");
        for (int i=0;i<5; i++) {
            Serial.print(output[i], HEX);
            if(i!=4) Serial.print(", ");
            
        }
        Serial.println();
        Serial.println();
        outputCount++;
        if(outputCount > 9)
        {
            outputCount = 0;
        }
    }
}
void ADS1299::activeChannel(int channel, bool en)
{
    int channel_reg_addr, value;
    channel_reg_addr = channel + 4;
    /* Read ChnSET register */
    value = RREG(channel_reg_addr);
    if (en)
    {
        value = (value & 0x7E); // Enable the channel.
    }
    else
    {
        value = value | 0x80; // disable the channel
        value = value | 0x01; // Input shorted (for offset or noise measurements).
    }
    WREG(channel_reg_addr, value) ;
}

void ADS1299::setGain(int channel, afe_gain_t gain)
{
    uint8_t channel_reg_addr, value;
    channel_reg_addr = channel + 4;

    /* Set channel gain */
    value = (value & (~ADS1299_GAIN_MASK)) | (gain << ADS1299_GAIN_POS);
    WREG(channel_reg_addr, value);

    /* Save the channel gain configuration to instance */
    // channel_config[channel-1].gain = gain;

    /* Calculate LSB value which will be used to convert RAW data into uV */
    afe_lsb_value = (float)(AFE_VREF_uV * 2) / 16777215; // LSB = (2 * Vref) / (2^24 -1)
    //calculated value = 0.5364418349529406
    switch (gain)
    {
    case AFE_GAIN_1:
        afe_lsb_value = afe_lsb_value / 1;
        break;
    case AFE_GAIN_2:
        afe_lsb_value = afe_lsb_value / 2;
        break;
    case AFE_GAIN_4:
        afe_lsb_value = afe_lsb_value / 4;
        break;
    case AFE_GAIN_6:
        afe_lsb_value = afe_lsb_value / 6;
        break;
    case AFE_GAIN_8:
        afe_lsb_value = afe_lsb_value / 8;
        break;
    case AFE_GAIN_12:
        afe_lsb_value = afe_lsb_value / 12;
        break;
    case AFE_GAIN_24:
        afe_lsb_value = afe_lsb_value / 24;
        break;
    default:
        break;
    }
}

void ADS1299::leadOffSet(int channel, bool p, bool n)
{
    uint8_t channel_mask;
    uint8_t loff_p_value, loff_n_value;

    channel_mask = 0x1 << (channel - 1);

    /* Read LOFF_SENSP and LOFF_SENSN register */
    loff_p_value = RREG(LOFF_SENSP);
    loff_n_value = RREG(LOFF_SENSN);

    /* set/clear lead off for positive signal of the channel */
    if (p)
    {
        loff_p_value |= channel_mask;
    }
    else
    {
        loff_p_value &= ~channel_mask;
    }

    /*  set/clear lead off for negative signal of the channel */
    if (n)
    {
        loff_n_value |= channel_mask;
    }
    else
    {
        loff_n_value &= ~channel_mask;
    }

    WREG(LOFF_SENSP, loff_p_value);
    WREG(LOFF_SENSN, loff_n_value);

    /* Save the channel active configuration to instance */
    // instance->channel_config[channel-1].positive_loff = p;
    // instance->channel_config[channel-1].negative_loff = n;

}

bool ADS1299::isAvailableData()
{
  return outputCount == 9;
}
long** ADS1299::dataExport()
{
  // Serial.println("Export to BLE Streaming");
  return outputData;
}
ADS1299::ADS1299()
{
}

ADS1299::~ADS1299()
{
}