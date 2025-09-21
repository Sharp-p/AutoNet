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
#include <unistd.h>
#include <libgen.h>
#include <string.h>
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

    // PLOTTING

    PLFLT x[len], y[len];
    PLFLT xmin = 0., xmax = 360., ymin = 0., ymax = 450.;

    
    Node* head = NULL;
    Node* tail = NULL;
    //bool accepting = true;

    const float rate = 360. / len;
    int valids = 0;
    for (int i = 0; i < len; i++) {
       // Node* temp = malloc(sizeof(Node));
       // if (!temp) {
       //     perror("[ERROR]Couldn't allocate memory for the node.");
       //     exit(1);
       // }
       // temp->next = NULL;

       // if (tail) {
       //     tail->next = temp;
       // }
       // else {
       //     head = temp;
       // }
       // tail = temp;

       // if(distances[i] == -1) { 
       //     if (accepting) {
       //         temp->val
       //     } 
       // }
        x[i] = (PLFLT) rate * (i+1);
        y[i] = (PLFLT) distances[i];
        
        //valids++;

        printf("Sto riempendo. i = %d - valids = %d, gradi = %f, distanza = %f\n", i, valids, x[i], y[i]);
    }

    // driver to save in PNG
    plsdev("pngcairo");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // creating path in which to save plot
    char filename[1024];
    
    snprintf(filename, sizeof(filename), "%s/plots/distance_%04d%02d%02d_%02d%02d%02d.png",
            PROJECT_DIR, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    plsfnam(filename);

    // overwriting the color 0 (used for the BG) of the color map0
    plscolbg(255, 251, 247);
    // overwriting the color 1 (used for the data) of the color map0
    plscol0a(1, 0, 0, 0, 1);

    plinit();

    plenv(xmin, xmax, ymin, ymax, 0 ,0);
    plsfont(PL_FCI_SERIF, PL_FCI_ITALIC, PL_FCI_MEDIUM);
    pllab("Gradi (°)", "Distanza (cm)", "Prova mappatura distanze");
    
    int i = 0;
    while (i < len) {
        while (i < len && y[i] < 0) i++;

        int start = i;
        while (i < len && y[i] >= 0) i++;

        int segment_len = i - start;
        if (segment_len > 1) {
            plline(segment_len, &x[start], &y[start]);
            plstring(segment_len, &x[start], &y[start], "•");
        }
    }

    //plstring(valids, x, y, "•");
    //plline(valids, x, y);

    plend();

    // TODO: salvare i dati in csv

    free(distances);

    return EXIT_SUCCESS;
}

