#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <netlib.h>
#include <time.h>
#include <sys/random.h>

bool neighbor_discover(const uint8_t n)
{
    uint8_t nCount = 0;
    bool transmit = true;
    uint8_t last = -1;
    uint8_t neighbours[MAX_NEIGHBORS];

    for (int round = 1; round < (int) log(log(n)); round++)
    {
        double p = pow(2.0, round - 1) / n;
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
                //listen for t seconds
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
    srand((unsigned int)time(nullptr));





    netStopRecvLoop();
    netDestroy();
	return EXIT_SUCCESS;
}