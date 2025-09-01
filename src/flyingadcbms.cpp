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
#include "hwdefs.h"
#include "cellmux.h"
#include "mcp3421.h"

#define HBRIDGE_DISCHARGE_VIA_LOWSIDE    0xF
#define HBRIDGE_DISCHARGE_VIA_HIGHSIDE   0x0
#define HBRIDGE_ALL_OFF                  0xA
#define HBRIDGE_UOUTP_TO_GND_UOUTN_TO_5V 0xC
#define HBRIDGE_UOUTP_TO_5V_UOUTN_TO_GND 0x3

Adc FlyingAdcBms::adc = MCP3421();

void FlyingAdcBms::Ms2Task()
{
   CellMux::Ms2Task();
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


