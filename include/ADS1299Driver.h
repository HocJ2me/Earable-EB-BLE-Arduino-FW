#ifndef ADS1299DRIVER_H
#define ADS1299DRIVER_H

#include <SPI.h>
#include "Arduino.h"
#include "configs.h"
#include "Definitions.h"


class ADS1299 {
    private:
        SPIClass * vspi = NULL;
    public:
        int outputCount = 0;
        void init();
        void reset();
        void start();
        void RREG(byte _address);
        void RREG(byte _address, byte _numRegistersMinusOne); 
        void WREG(byte _address, byte _value); //

        void getDeviceID();
        byte spiCommand(byte);
        void logByte2Serial(byte, byte);
        void SPI_csSetLow();
        void SPI_csSetHigh();
        byte SPI_xfer(byte);
        void updateData();
        ADS1299(/* args */);
        ~ADS1299();
};
#endif