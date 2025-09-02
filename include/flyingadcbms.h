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
#ifndef FLYINGADCBMS_H
#define FLYINGADCBMS_H
#include <stdint.h>
#include "bmshardware.h"
#include "adc.h"
#include "cellmux.h"

#define MAX_CELLS 108

class FlyingAdcBms : public BmsHardware
{
   public:
      static void Init();
      static void Ms2Task();
      static void Ms25Task();
      static void Ms100Task();

      // static float cell_voltages[MAX_CELLS];
      // static FlyingAdcBms::BalanceStatus cell_balance_status[MAX_CELLS];
      // static float umax, umin, uavg;

   private:
      static BalanceStatus SetBalancing(BmsAlgo::BalanceCommand cmd);
      static void MuxReadyCallback();

      static Adc adc;
      static CellMux mux;

      // static uint8_t selectedChannel, previousChannel;
};

#endif // FLYINGADCBMS_H
