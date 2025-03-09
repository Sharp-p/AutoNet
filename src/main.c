//
// Created by f3m on 09/03/25.
//

#include "main.h"


void temp(const PacketType_t type, const size_t len, u_char *data)
{
    D_Print("Packet Type: %d\n", type);
    D_Print("Packet Length: %d\n", len);
    D_Print("Packet Data: %s\n", data);
}


int main()
{
#ifdef Debug
    D_Print("Launched in Debug Mode!\n");
#endif
    if (initPcap() || createHandle("wlan1") || activateHandle())
        return EXIT_FAILURE;

    setCallback(temp);
    loopPcap();

    for (int i = 0; i < 200000; ++i)
    {
        D_Print("i: %d\n", i);
    }
    stopPcap();

    return EXIT_SUCCESS;
}
