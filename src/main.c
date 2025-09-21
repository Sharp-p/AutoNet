//
// Created by f3m on 09/03/25.
//

#include "main.h"

//#include <logger.h>
#include <networkMan.h>
#include <stdio.h>
#include <stdlib.h>
#include <plplot/plplot.h>
#include <time.h>
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

    // DO NOT forget initialization
    int len = 0;
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

    // driver to save in PNG
    plsdev("pngcairo");
    
    char filename[256];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    snprintf(filename, sizeof(filename), "distance_%04d%02d%02d_%02d%02d%02d.png",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour,tm.tm_min, tm.tm_sec);

    plsfnam(filename);

    plinit();

    plenv(xmin, xmax, ymin, ymax, 0, 0);
    pllab("Gradi", "Distanza", "Prova mappatura distanze");

    plline(len, x, y);

    plend();

    free(distances);

    return EXIT_SUCCESS;
}

