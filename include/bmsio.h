/*
 * This file is part of the FlyingADCBMS project.
 *
 * Copyright (C) 2025 Johannes Huebner <dev@johanneshuebner.com>
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
#ifndef BMSIO_H
#define BMSIO_H

#include "bmsfsm.h"
#include "bmsalgo.h"
#include "flyingadcbms.h"

#define NO_TEMP    127


class BmsIO
{
   public:
      static void Init();
      static void ReadTemperatures();
      static void SwitchMux();
      static void ReadCellVoltages();
      static void TestReadCellVoltage(int chan, BmsAlgo::BalanceCommand cmd);
      static void MeasureCurrent();
      static void SetBmsFsm(BmsFsm* b) { bmsFsm = b; }

      static BmsHardware bmshardware;

   private:
      static void Accumulate(float sum, float min, float max, float avg);
      static BmsFsm* bmsFsm;
      static int muxRequest;
};

#endif // BMSIO_H
