#ifndef BMSHARDWARE_H
#define BMSHARDWARE_H

class BMSHardware
{
    public:
        enum BalanceCommand { BAL_OFF, BAL_CHARGE, BAL_DISCHARGE };
        enum BalanceStatus  { STT_OFF, STT_DISCHARGE, STT_CHARGEPOS, STT_CHARGENEG };
};

#endif