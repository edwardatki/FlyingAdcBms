#ifndef PCA9536_H
#define PCA9536_H

#include "bitbangi2c.h"

class PCA9536
{
    public:
        PCA9536(BitBangI2C *_i2c_interface, uint8_t _address) : i2c_interface(_i2c_interface), address(_address) {};
        void SetAsOutputs();
        void WriteOutputs(uint8_t value);

    private:
        BitBangI2C *i2c_interface;
        uint8_t address;
};



#endif // PCA9536_H
