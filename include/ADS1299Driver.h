#ifndef ADS1299DRIVER_H
#define ADS1299DRIVER_H

#include <SPI.h>
#include "Arduino.h"
#include "string.h"
#include "configs.h"
#include "Definitions.h"

typedef enum
{
  AFE_GAIN_1 = 0,
  AFE_GAIN_2,
  AFE_GAIN_4,
  AFE_GAIN_6,
  AFE_GAIN_8,
  AFE_GAIN_12,
  AFE_GAIN_24,
  AFE_GAIN_INVALID,
} afe_gain_t;

#define AFE_NUM_CHANNEL (6)
#define ADS1299_GAIN_POS 4
#define ADS1299_GAIN_MASK (0x07 << ADS1299_GAIN_POS)
#define AFE_VREF_uV             4500000
#define AFE_DEFAULT_CONFIG_GAIN         AFE_GAIN_6

// #define DISPLAY_DATA_WHILE_STREAMING


class ADS1299 {
    private:
        SPIClass * vspi = NULL;
    public:
        int outputCount = 0;
        long **outputData;
        float afe_lsb_value = 0;

        void init();
        void reset();
        void start();
        byte RREG(byte _address);
        void RREG(byte _address, byte _numRegistersMinusOne); 
        void WREG(byte _address, byte _value); //

        void getDeviceID();
        byte spiCommand(byte);
        void logByte2Serial(byte, byte);
        void SPI_csSetLow();
        void SPI_csSetHigh();
        byte SPI_xfer(byte);
        bool updateData();


        void activeChannel(int channel, bool en);
        void setGain(int channel, afe_gain_t gain);
        void leadOffSet(int channel, bool p, bool n);
        bool isAvailableData();
        long** dataExport();
        ADS1299(/* args */);
        ~ADS1299();
};
#endif