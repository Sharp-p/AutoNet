//
// Created by f3m on 09/03/25.
//

#include "main.h"


int init()
{
    D_Print("Initializing BNet...\n");
    if (initBNet())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int main()
{
#ifdef Debug
    D_Print("Launched in Debug Mode!\n");
#endif

    //TODO: init di BNet
    //TODO: conteggio nodi
    //TODO: init protocollo Heartbeat
    //TODO: stabilire ordine (leader election?ID?)
    //TODO: init di Distance con l'ordine di inizio
    //TODO: init di Ultra wideband con l'ordine di inizio

    if (init())
        return EXIT_FAILURE;


    return EXIT_SUCCESS;
}

