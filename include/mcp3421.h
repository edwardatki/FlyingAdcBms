#ifndef MCP3421_H
#define MCP3421_H

#include "adc.h"

class MCP3421 : public Adc
{
    public:
        static void StartAdc();
        static float GetResult();
};



#endif // MCP3421_H
