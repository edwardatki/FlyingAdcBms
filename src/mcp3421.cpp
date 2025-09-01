#include "mcp3421.h"
#include "bitbangi2c.h"

#define ADC_ADDR        0x68

//ADC configuration register defines (only those we need)
#define ADC_START       0x80
#define ADC_RATE_240SPS 0x0
#define ADC_RATE_60SPS  0x4
#define ADC_RATE_15SPS  0x8

void MCP3421::StartAdc()
{
   uint8_t byte = ADC_START | ADC_RATE_60SPS; //Start in manual mode with 14 bit/60 SPS resolution
   BitBangI2C::SendRecvI2C(ADC_ADDR, WRITE, &byte, 1);
//    previousChannel = selectedChannel; //now we can switch the mux and still read the correct result
}

float MCP3421::GetResult()
{
   uint8_t data[3];
   BitBangI2C::SendRecvI2C(ADC_ADDR, READ, data, 3);
   int32_t adc = (((int16_t)(data[0] << 8)) + data[1]);
   float result = adc;
   //Odd channels are connected to ADC with reversed polarity
//    if (previousChannel & 1) result = -result;

   return result;
}