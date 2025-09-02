#include "pca9536.h"
#include "bitbangi2c.h"

#define OUTPUTS_REGISTER 0x01
#define PINMODE_REGISTER 0x03

void PCA9536::SetAsOutputs() {
    uint8_t data[2] = { PINMODE_REGISTER, 0x0 /* All pins as output */};
    this->i2c_interface->SendRecvI2C(this->address, WRITE, data, 2);
}

void PCA9536::WriteOutputs(uint8_t value) {
    uint8_t data[2] = { PINMODE_REGISTER, value};
    this->i2c_interface->SendRecvI2C(this->address, WRITE, data, 2);
}