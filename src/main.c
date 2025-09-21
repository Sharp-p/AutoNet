//
// Created by f3m on 09/03/25.
//

#include "main.h"

//#include <logger.h>
#include <networkMan.h>
#include <stdio.h>
#include <stdlib.h>
#include <plplot/plplot.h>
#include "distance.h"


int init()
{
//    logD(stdout, "Initializing BNet...\n");
    // if (initBNet())
    //     return EXIT_FAILURE;
    //
    // return EXIT_SUCCESS;
}

int main()
{
#ifdef Debug
//    logD(stdout, "Launched in Debug Mode!\n");
#endif

    //TODO: init di BNet
    //TODO: conteggio nodi
    //TODO: init failure detector
    //TODO: stabilire ordine (leader election?ID?)
    //TODO: init di Distance con l'ordine di inizio
    //TODO: init di Ultra wideband con l'ordine di inizio

    // if (init())
    //     return EXIT_FAILURE;

    int len;
    float* distances = flightTime(&len);

    for (int i = 0; i < len; i++)
    {
        printf("Distance %d: %f\n", i, distances[i]);
    }

    PLFLT x[len], y[len];
    PLFLT xmin = 0., xmax = 360., ymin = 0., ymax = 450.;

    float rate = 360 / len;

    for (int i = 0; i < len; i++) {
        x[i] = (PLFLT) rate * i;
        y[i] = (PLFLT) distances[i];
    }

    plinit();

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("Gradi", "Distanza", "Prova mappatura distanze");

    plline(len, x, y);

    plend();

    return EXIT_SUCCESS;
}

