#ifndef CELLMUX_H
#define CELLMUX_H

#include <stdint.h>

class CellMux
{
    public:
        static void Init();
        static void MuxRequestChannel(uint8_t channel);
        static void MuxOff();
        static void SelectChannel(uint8_t channel);

        static void Ms2Task();

    private:
        static uint8_t selectedChannel, previousChannel;
        static int muxRequest;
};



#endif // CELLMUX_H
