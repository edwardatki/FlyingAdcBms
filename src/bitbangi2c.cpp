#include "bitbangi2c.h"
#include "digio.h"
#include "hwdefs.h"

uint8_t BitBangI2C::i2cdelay = 30;

#define DELAY() for (volatile int _ctr = 0; _ctr < i2cdelay; _ctr++)

static bool lock = false;

void BitBangI2C::SendRecvI2C(uint8_t address, bool read, uint8_t* data, uint8_t len)
{
   if (lock) return;

   lock = true;

   BitBangI2C::BitBangI2CStart();

   address <<= 1;
   address |= read;

   BitBangI2C::BitBangI2CByte(address, false);

   for (int i = 0; i < len; i++)
      data[i] = BitBangI2C::BitBangI2CByte(read ? 0xFF : data[i], i != (len - 1) && read);

   BitBangI2C::BitBangI2CStop();

   lock = false;
}

void BitBangI2C::BitBangI2CStart()
{
   BitBangI2C::do_pin.Clear(); //Generate start. First SDA low, then SCL
   DELAY();
   BitBangI2C::scl_pin.Clear();
}

uint8_t BitBangI2C::BitBangI2CByte(uint8_t byte, bool ack)
{
   uint8_t byteRead = 0;

   BitBangI2C::scl_pin.Clear();
   DELAY();

   for (int i = 16; i >= 0; i--)
   {
      if (byte & 0x80 || (i < 1 && !ack)) BitBangI2C::do_pin.Set();
      else BitBangI2C::do_pin.Clear();
      DELAY();
      BitBangI2C::scl_pin.Toggle();
      if (i & 1) {
         byte <<= 1; //get next bit at falling edge
      }
      else if (i > 0)
      {
         byteRead <<= 1;
         byteRead |= BitBangI2C::di_pin.Get(); //Read data at rising edge
      }
   }
   DELAY();

   return byteRead;
}

void BitBangI2C::BitBangI2CStop()
{
   BitBangI2C::scl_pin.Clear();
   DELAY();
   BitBangI2C::do_pin.Clear(); //data low
   DELAY();
   BitBangI2C::scl_pin.Set();
   DELAY();
   BitBangI2C::do_pin.Set(); //data high -> STOP
   DELAY();
}