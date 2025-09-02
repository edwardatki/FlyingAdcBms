#ifndef CELLMUX_H
#define CELLMUX_H

#include <stdint.h>

class CellMux
{
    public:
        static void Init();
        static void MuxRequestChannel(uint8_t channel);
        static void MuxOff();

        static void Ms2Task();

        static bool ready;

    private:
        static void SelectChannel(uint8_t channel);

        static uint8_t selectedChannel, previousChannel;
        static int muxRequest;
};



#endif // CELLMUX_H
