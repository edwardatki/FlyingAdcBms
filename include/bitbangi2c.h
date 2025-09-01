#ifndef BITBANGI2C_H
#define BITBANGI2C_H

#include <stdint.h>

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
};



#endif // BITBANGI2C