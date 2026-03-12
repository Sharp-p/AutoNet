#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <netlib.h>
#include <time.h>
#include <sys/random.h>
#include <netqueue.h>

Queuelist_t queuelist;

void callback(uint8_t *buf, size_t size)
{
    if (buf[0] == 0x01)
        pushQueue(&queuelist.discovery, buf, size);

}

bool listen(const int millis)
{
    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_t sec = end.tv_sec - start.tv_sec;
    long nsec = end.tv_nsec - start.tv_nsec;

    if (nsec < 0)
    {
        sec--;
        nsec += 1000000000L;
    }

    uint64_t elapsed_ms = (uint64_t)sec * 1000ULL + (uint64_t)nsec / 1000000ULL;

    while (elapsed_ms < millis)
    {
        uint8_t *buf;
        size_t size;
        if (popQueue(&queuelist.discovery, &buf, &size) == EXIT_SUCCESS)
        {
            //controlla pacchetto e ritorna true o false in caso
            return true;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (nsec < 0)
        {
            sec--;
            nsec += 1000000000L;
        }
        elapsed_ms = (uint64_t)sec * 1000ULL + (uint64_t)nsec / 1000000ULL;
    }
    return false;
}

bool neighbor_discover(const uint8_t n)
{
    uint8_t nCount = 0;
    bool transmit = true;
    uint8_t last = -1;
    uint8_t neighbours[MAX_NEIGHBORS];

    for (int round = 1; round < (int) log(log(n)); round++)
    {
        const double p = pow(2.0, round - 1) / n;
        for (int time_slot = 1; time_slot <= 8 * n / pow(2.0, round) + 8 * log(2 + n); time_slot++)
        {
            uint32_t random;
            getrandom(&random, sizeof(uint32_t), 0);
            if (transmit && (double) random / UINT32_MAX < p)
            {
                //send
            }
            else
            {
                transmit |= listen(TIME_SLOT);
            }
        }
    }
    //switch to aloha
}

int main()
{
    if (netInit() ||
        netStartRecvLoop())
        return EXIT_FAILURE;
    setCallback(callback);




    netStopRecvLoop();
    netDestroy();
    destroyQueuelist(&queuelist);
	return EXIT_SUCCESS;
}