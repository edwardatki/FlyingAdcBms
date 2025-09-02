/*
 * This file is part of the FlyingAdcBms project.
 *
 * Copyright (C) 2023 Johannes Huebner <openinverter.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "bmsalgo.h"
#include "flyingadcbms.h"
#include "digio.h"
#include "digio_prj.h"
#include "hwdefs.h"
#include "cellmux.h"
#include "mcp3421.h"
#include "params.h"
#include "bmsfsm.h"

#define HBRIDGE_DISCHARGE_VIA_LOWSIDE    0xF
#define HBRIDGE_DISCHARGE_VIA_HIGHSIDE   0x0
#define HBRIDGE_ALL_OFF                  0xA
#define HBRIDGE_UOUTP_TO_GND_UOUTN_TO_5V 0xC
#define HBRIDGE_UOUTP_TO_5V_UOUTN_TO_GND 0x3

BitBangI2C i2c_interface = BitBangI2C(&DigIo::i2c_scl, &DigIo::i2c_do, &DigIo::i2c_di);

Adc FlyingAdcBms::adc = MCP3421(&i2c_interface, 0x68, MCP3421::Rate::RATE_60SPS_14BIT);
CellMux FlyingAdcBms::mux = CellMux();

void FlyingAdcBms::Init() {
   if (hwRev == HW_23 || hwRev == HW_24) i2c_interface.delay = 5;
   else i2c_interface.delay = 30;
   
   mux.readyCallback = &FlyingAdcBms::MuxReadyCallback;
}

void FlyingAdcBms::MuxReadyCallback() {
   adc.StartAdc();
}

void FlyingAdcBms::Ms2Task()
{
   mux.Ms2Task();
}


/** \brief Read cell voltages and set balancing. Must be called in 25 ms interval */
void FlyingAdcBms::Ms25Task()
{
   const int totalBalanceCycles = 30;
   static uint8_t chan = 0, balanceCycles = 0;
   static float sum = 0, min = 8000, max = 0;
   int balMode = Param::GetInt(Param::balmode);
   bool balance = Param::GetInt(Param::opmode) == BmsFsm::IDLE && Param::GetFloat(Param::uavg) > Param::GetFloat(Param::ubalance) && BAL_OFF != balMode;
   FlyingAdcBms::BalanceStatus bstt;

   if (balance)
   {
      if (balanceCycles == 0)
      {
         balanceCycles = totalBalanceCycles; //this leads to switching to next channel below
      }
      else
      {
         balanceCycles--;
      }

      if (balanceCycles > 0 && balanceCycles < (totalBalanceCycles - 1))
      {
         BmsAlgo::BalanceCommand balanceCommand = BmsAlgo::SelectBalancing(
            (BmsAlgo::BalanceMode)Param::GetInt(Param::balmode), 
            Param::GetFloat((Param::PARAM_NUM)(Param::u0 + chan)),
            Param::GetFloat(Param::umin),
            Param::GetFloat(Param::umax),
            Param::GetFloat(Param::uavg)
         );
         bstt = FlyingAdcBms::SetBalancing(balanceCommand);
         Param::SetInt((Param::PARAM_NUM)(Param::u0cmd + chan), bstt);
      }
      else
      {
         FlyingAdcBms::SetBalancing(BmsAlgo::BalanceCommand::BAL_OFF);
      }
   }
   else
   {
      balanceCycles = totalBalanceCycles;
      bstt = FlyingAdcBms::SetBalancing(BmsAlgo::BalanceCommand::BAL_OFF);
      Param::SetInt((Param::PARAM_NUM)(Param::u0cmd + chan), bstt);
   }

   //Read cell voltage when balancing is turned off (at start of balancing cycles)
   if (balanceCycles == totalBalanceCycles)
   {
      float gain = Param::GetFloat(Param::gain);
      int numChan = Param::GetInt(Param::numchan);
      bool even = (chan & 1) == 0;

      if (chan == 0)
         gain *= 1 + Param::GetFloat(Param::correction0) / 1000000.0f;
      else if (chan == 1)
         gain *= 1 + Param::GetFloat(Param::correction1) / 1000000.0f;
      else if (chan == 15)
         gain *= 1 + Param::GetFloat(Param::correction15) / 1000000.0f;

      //Read ADC result before mux change
      float udc = adc.GetResult() * (gain / 1000.0f);

      FlyingAdcBms::cell_voltages[chan] = udc;
      Param::SetFloat((Param::PARAM_NUM)(Param::u0 + chan), udc);

      min = MIN(min, udc);
      max = MAX(max, udc);
      sum += udc;

      //First we sweep across all even channels: 0, 2, 4,...
      if (even && (chan + 2) < numChan)
         chan += 2;
      //After reaching the furthest even channel (say 12) we either change over to a higher odd channel
      else if (even && (chan + 1) < numChan)
         chan++;
      //or lower odd channel
      else if (even)
         chan--;
      //Now we sweep across all odd channels until we reach 1
      else if (chan > 1)
         chan -= 2;
      //We have now reached chan 1. Accumulate values and restart at chan 0
      else
      {
         chan = 0;
         // Accumulate(sum, min, max, sum / numChan);
         // TODO: Callback to BmsIO

         min = 8000;
         max = 0;
         sum = 0;
      }
      // This instructs the SwitchMux task to change channel, with dead time
      mux.MuxRequestChannel(chan);
   }
}

FlyingAdcBms::BalanceStatus FlyingAdcBms::SetBalancing(BmsAlgo::BalanceCommand cmd)
{
   // BalanceStatus stt = STT_OFF;
   // uint8_t data[2] = { 0x3 /* pin mode register */, 0x0 /* All pins as output */};
   // SendRecvI2C(DIO_ADDR, WRITE, data, 2);

   // data[0] = 0x1; //output port register

   // switch (cmd)
   // {
   // case BmsAlgo::BalanceCommand::BAL_OFF:
   //    data[1] = HBRIDGE_ALL_OFF;
   //    break;
   // case BmsAlgo::BalanceCommand::BAL_DISCHARGE:
   //    if (hwRev == HW_24) //has inline resistors on all channels
   //       data[1] = HBRIDGE_DISCHARGE_VIA_HIGHSIDE;
   //    else
   //       data[1] = HBRIDGE_DISCHARGE_VIA_LOWSIDE;
   //    stt = STT_DISCHARGE;
   //    break;
   // case BmsAlgo::BalanceCommand::BAL_CHARGE:
   //    //odd channel: connect UOUTP to GNDA and UOUTN to VCCA
   //    //even channel: connect UOUTP to VCCA and UOUTN to GNDA
   //    data[1] = selectedChannel & 1 ? HBRIDGE_UOUTP_TO_GND_UOUTN_TO_5V : HBRIDGE_UOUTP_TO_5V_UOUTN_TO_GND;
   //    stt = selectedChannel & 1 ? STT_CHARGENEG : STT_CHARGEPOS;
   // }

   // SendRecvI2C(DIO_ADDR, WRITE, data, 2);

   // return stt;
}


