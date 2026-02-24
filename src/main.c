#include <stdio.h>
#include <autonet.h>
#include <string.h>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: test 0/1 (0 = send, 1 = receive)\n");
        return EXIT_FAILURE;
    }

    if (strncmp(argv[1], "0", 1) == 0)
    {
        constexpr char payload[] = "Hello World!";
        netBroadcast(payload, strlen(payload));
        printf("Sent!\n");
    }
    else if (strncmp(argv[1], "1", 1) == 0)
    {
        netRecvLoop();
    }
    else
    {
        fprintf(stderr, "Usage: test 0/1 (0 = send, 1 = receive)\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
