//
// Created by f3m on 20/03/25.
//

#include "networkMan.h"
#include <BNet.h>
#include <stdlib.h>
#include <logger.h>

void callback(PacketType_t type, size_t size, const u_char *data, void *usrData)
{
    // D_Print("IMPLEMENT THIS CALLBACK!\n");
    logD(stdout, "IMPLEMENT THIS CALLBACK!\n");
}

int initBNet()
{
    if (initPcap(stderr, stdout))
        return EXIT_FAILURE;
    setCallback(callback, NULL);
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
