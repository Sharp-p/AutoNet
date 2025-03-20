//
// Created by f3m on 20/03/25.
//

#include "networkMan.h"

void callback(PacketType_t type, size_t size, u_char *data)
{
    D_Print("IMPLEMENT THIS CALLBACK!\n");
}

int initBNet()
{
    if (initPcap())
        return EXIT_FAILURE;
    setCallback(callback);
    if (createHandle("wlan1"))
        return EXIT_FAILURE;
    if (activateHandle())
        return EXIT_FAILURE;
    if (loopPcap())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


void cleanBNet()
{
    stopPcap();
}
