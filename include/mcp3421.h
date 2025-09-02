#ifndef MCP3421_H
#define MCP3421_H

#include <stdint.h>
#include "adc.h"
#include "bitbangi2c.h"

class MCP3421 : public Adc
{
    public:
        enum class Rate : uint8_t { RATE_240SPS_12BIT=0x0, RATE_60SPS_14BIT=0x4, RATE_15SPS_16BIT=0x8 };

        MCP3421(BitBangI2C *_i2c_interface, uint8_t _address, Rate _rate) : i2c_interface(_i2c_interface), address(_address), rate(_rate) {};
        void StartAdc();
        float GetResult();
    
    private:
        BitBangI2C *i2c_interface;
        uint8_t address;
        Rate rate;
};



#endif // MCP3421_H
