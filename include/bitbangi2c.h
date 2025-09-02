#ifndef BITBANGI2C_H
#define BITBANGI2C_H

#include <stdint.h>
#include "digio.h"

#define READ            true
#define WRITE           false

class BitBangI2C
{
    public:
        BitBangI2C(DigIo *_scl_pin, DigIo *_do_pin, DigIo *_di_pin) : scl_pin(_scl_pin), do_pin(_do_pin), di_pin(_di_pin) {};

        void SendRecvI2C(uint8_t address, bool read, uint8_t* data, uint8_t len);
        void BitBangI2CStart();
        uint8_t BitBangI2CByte(uint8_t byte, bool ack);
        void BitBangI2CStop();

        uint8_t delay = 30;

    private:
        DigIo *scl_pin, *do_pin, *di_pin;
};



#endif // BITBANGI2C