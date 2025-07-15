//
// Created by f3m on 09/03/25.
//

#include "main.h"

#include <logger.h>
#include <networkMan.h>
#include <stdlib.h>


int init()
{
    logD(stdout, "Initializing BNet...\n");
    if (initBNet())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int main()
{
#ifdef Debug
    logD(stdout, "Launched in Debug Mode!\n");
#endif

    //TODO: init di BNet
    //TODO: conteggio nodi
    //TODO: init failure detector
    //TODO: stabilire ordine (leader election?ID?)
    //TODO: init di Distance con l'ordine di inizio
    //TODO: init di Ultra wideband con l'ordine di inizio

    if (init())
        return EXIT_FAILURE;


    return EXIT_SUCCESS;
}

