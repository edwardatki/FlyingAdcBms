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
   DigIo::i2c_do.Clear(); //Generate start. First SDA low, then SCL
   DELAY();
   DigIo::i2c_scl.Clear();
}

uint8_t BitBangI2C::BitBangI2CByte(uint8_t byte, bool ack)
{
   uint8_t byteRead = 0;

   DigIo::i2c_scl.Clear();
   DELAY();

   for (int i = 16; i >= 0; i--)
   {
      if (byte & 0x80 || (i < 1 && !ack)) DigIo::i2c_do.Set();
      else DigIo::i2c_do.Clear();
      DELAY();
      DigIo::i2c_scl.Toggle();
      if (i & 1) {
         byte <<= 1; //get next bit at falling edge
      }
      else if (i > 0)
      {
         byteRead <<= 1;
         byteRead |= DigIo::i2c_di.Get(); //Read data at rising edge
      }
   }
   DELAY();

   return byteRead;
}

void BitBangI2C::BitBangI2CStop()
{
   DigIo::i2c_scl.Clear();
   DELAY();
   DigIo::i2c_do.Clear(); //data low
   DELAY();
   DigIo::i2c_scl.Set();
   DELAY();
   DigIo::i2c_do.Set(); //data high -> STOP
   DELAY();
}