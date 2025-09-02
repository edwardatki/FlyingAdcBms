#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "digio.h"
#include "hwdefs.h"
#include "cellmux.h"
#include "bitbangi2c.h"
#include "mcp3421.h"

#define DIO_ADDR        0x41

uint8_t CellMux::selectedChannel = 0;
uint8_t CellMux::previousChannel = 0;
int CellMux::muxRequest = -1;
void(*CellMux::readyCallback)();

/** \brief Mux control function. Must be called in 2 ms interval */
void CellMux::Ms2Task()
{
   static int channel = -1;
   static bool startAdc = false;

   //t=0 ms: On a mux change request first completely turn off mux
   if (muxRequest >= 0)
   {
      CellMux::MuxOff();
      channel = muxRequest;
      muxRequest = -1;
   }
   //t=2 ms: switch to requested channel
   else if (channel >= 0)
   {
      CellMux::SelectChannel(channel);
      channel = -1;
      startAdc = true;
   }
   //t=4 ms: start ADC
   else if (startAdc)
   {
      if (readyCallback != nullptr) CellMux::readyCallback();
      startAdc = false;
   }
   //t=21 ms: ADC conversion is finished
   //t=25 ms: ADC conversion result is read
}

void CellMux::MuxRequestChannel(uint8_t channel)
{
   //This instructs the SwitchMux task to change channel, with dead time
   muxRequest = channel;
}

#ifdef HWV1

void CellMux::Init()
{
   uint8_t data[] = { 0x3, 0x0 };
   BitBangI2C::SendRecvI2C(DIO_ADDR, WRITE, data, 2);
   gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
}

//Mux control words
#define MUX_OFF         0x0080
#define MUX_SELECT      0x80C0

void CellMux::MuxOff()
{
   //Turn off mux
   spi_xfer(SPI1, MUX_OFF);
   SetBalancing(BAL_OFF);
   gpio_clear(GPIOB, GPIO0);
}

void CellMux::SelectChannel(uint8_t channel)
{
   gpio_set(GPIOB, GPIO0);
   selectedChannel = channel;
   //Select MUX channel with deadtime insertion
   spi_xfer(SPI1, MUX_SELECT | channel);
}

#else

void CellMux::Init()
{
   // uint8_t data[2] = { 0x3 /* pin mode register */, 0x0 /* All pins as output */};
   // BitBangI2C::SendRecvI2C(DIO_ADDR, WRITE, data, 2);
   gpio_clear(GPIOB, 255);
   gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, 255);
}

void CellMux::MuxOff()
{
   //Turn off mux
   gpio_clear(GPIOB, 255);
}

void CellMux::SelectChannel(uint8_t channel)
{
   //Turn off all channels
   gpio_clear(GPIOB, 255);

   if (channel > 15) return;

   selectedChannel = channel;

   //Example Chan8:  turn on G8 (=even mux word 4) and G9 (odd mux word 4)
   //Example Chan9:  turn on G10 (=even mux word 5) and G9 (odd mux word 4)
   //Example Chan15: turn on G16 via GPIOB3 (=even mux word 8) and G15 via Decoder (odd mux word 7)
   uint8_t evenMuxWord = (channel / 2) + (channel & 1);
   uint8_t oddMuxWord = (channel / 2) << 4;
   gpio_set(GPIOB, evenMuxWord | oddMuxWord | GPIO7);
}

#endif // V1HW
