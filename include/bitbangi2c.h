#ifndef BITBANGI2C_H
#define BITBANGI2C_H

#include <stdint.h>
#include "digio.h"

#define READ            true
#define WRITE           false

class BitBangI2C
{
    public:
        static void SendRecvI2C(uint8_t address, bool read, uint8_t* data, uint8_t len);
        static void BitBangI2CStart();
        static uint8_t BitBangI2CByte(uint8_t byte, bool ack);
        static void BitBangI2CStop();

        static uint8_t i2cdelay;

    private:
        static DigIo scl_pin, do_pin, di_pin;
};



#endif // BITBANGI2C