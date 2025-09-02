#include "mcp3421.h"
#include "bitbangi2c.h"

//ADC configuration register defines (only those we need)
#define ADC_START       0x80

void MCP3421::StartAdc()
{
   //Start in manual mode with chosen rate/resolution
   uint8_t byte = ADC_START | (uint8_t)this->rate;
   this->i2c_interface->SendRecvI2C(this->address, WRITE, &byte, 1);
//    previousChannel = selectedChannel; //now we can switch the mux and still read the correct result
}

float MCP3421::GetResult()
{
   uint8_t data[3];
   this->i2c_interface->SendRecvI2C(this->address, READ, data, 3);
   int32_t adc = (((int16_t)(data[0] << 8)) + data[1]);
   float result = adc;

   return result;
}