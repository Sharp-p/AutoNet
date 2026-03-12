#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>

int s = 0;
bool stop = true;
pthread_t recv_thread;
uint8_t thread_result;
void (*callback)(uint8_t *, size_t) = nullptr;


int netInit()
{
    s = socket(AF_PACKET, SOCK_DGRAM, htons(ETHERTYPE));
    if (s < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct ifreq ifr = {};
    snprintf(ifr.ifr_name, IFNAMSIZ, "%s", INTERFACE_NAME);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
        return EXIT_FAILURE;

    struct sockaddr_ll bind_addr = {};
    bind_addr.sll_family   = AF_PACKET;
    bind_addr.sll_protocol = htons(ETHERTYPE);
    bind_addr.sll_ifindex  = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) < 0)
    {
        perror("bind");
        close(s);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void setCallback(void (*cb)(uint8_t *, size_t))
{
    callback = cb;
}


int netSend(const char *payload, const size_t len, const uint8_t *addr)
{
    struct sockaddr_ll sa = {};
    sa.sll_halen = ETH_ALEN;
    memcpy(sa.sll_addr, addr, 6);
    if (sendto(s, payload, len, 0, (struct sockaddr*)&sa, sizeof(sa)) < 0)
    {
        perror("sendto");
        close(s);
        return EXIT_FAILURE;
    }

    close(s);
    return EXIT_SUCCESS;
}
int netBroadcast(const char *payload, const size_t len)
{
    const uint8_t dst[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    return netSend(payload, len, dst);
}


void *netRecvLoop(void *args)
{
    while (!stop)
    {
        uint8_t buf[2048];

        struct sockaddr_ll from = {};
        socklen_t fromlen = sizeof(from);

        const ssize_t n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &from, &fromlen);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            thread_result = EXIT_FAILURE;
            return nullptr;
        }

        if (from.sll_halen != ETH_ALEN || from.sll_protocol != htons(ETHERTYPE))
            continue;

        uint8_t *new_buf = malloc(n);
        memcpy(new_buf, buf, n);
        if (callback != nullptr)
            callback(new_buf, n);
    }
    thread_result = EXIT_SUCCESS;
    return nullptr;
}

int netStartRecvLoop()
{
    printf("Listening on " INTERFACE_NAME " for EtherType 0x%04x ...\n", ETHERTYPE);
    stop = false;

    if (pthread_create(&recv_thread, nullptr, netRecvLoop, nullptr))
    {
        stop = true;
        printf("Error while creating thread\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
int netStopRecvLoop()
{
    stop = true;
    pthread_join(recv_thread, nullptr);
    recv_thread = 0;
    return EXIT_SUCCESS;
}


void netDestroy()
{
    if (!stop)
        netStopRecvLoop();
    close(s);
    s = 0;
    recv_thread = 0;
    callback = nullptr;
}
