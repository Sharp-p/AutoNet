#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

int netBroadcast(const char *payload, const size_t len)
{
    const char *ifname = "wlan0";
    const uint8_t dst[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

    const int s = socket(AF_PACKET, SOCK_DGRAM, htons(0x88B5));
    if (s < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct ifreq ifr = {};
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("SIOCGIFINDEX");
        return EXIT_FAILURE;
    }

    struct sockaddr_ll sa = {};
    sa.sll_family  = AF_PACKET;
    sa.sll_ifindex = ifr.ifr_ifindex;
    sa.sll_halen   = ETH_ALEN;
    memcpy(sa.sll_addr, dst, 6);

    if (sendto(s, payload, len, 0, (struct sockaddr*)&sa, sizeof(sa)) < 0)
    {
        perror("sendto");
        close(s);
        return EXIT_FAILURE;
    }

    close(s);
    return EXIT_SUCCESS;
}



static int get_ifindex(const int s, const char *ifname) {
    struct ifreq ifr = {0};
    snprintf(ifr.ifr_name, IFNAMSIZ, "%s", ifname);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) return -1;
    return ifr.ifr_ifindex;
}

int netRecvLoop()
{
    const char *ifname = "wlan0";
    constexpr uint16_t proto = 0x88B5;

    // SOCK_DGRAM: ricevi il "payload" (senza header Ethernet), filtrato per protocollo
    const int s = socket(AF_PACKET, SOCK_DGRAM, htons(proto));
    if (s < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    const int ifindex = get_ifindex(s, ifname);
    if (ifindex < 0)
    {
        perror("SIOCGIFINDEX");
        close(s);
        return EXIT_FAILURE;
    }

    struct sockaddr_ll bind_addr = {};
    bind_addr.sll_family   = AF_PACKET;
    bind_addr.sll_protocol = htons(proto);
    bind_addr.sll_ifindex  = ifindex;

    if (bind(s, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) < 0)
    {
        perror("bind");
        close(s);
        return EXIT_FAILURE;
    }

    printf("Listening on %s for EtherType 0x%04x ...\n", ifname, proto);

    for (;;)
    {
        uint8_t buf[2048];

        struct sockaddr_ll from = {};
        socklen_t fromlen = sizeof(from);

        const ssize_t n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, &fromlen);
        if (n < 0)
        {
            if (errno == EINTR) continue;
            perror("recvfrom");
            break;
        }

        if (from.sll_halen == ETH_ALEN)
        {
            printf("From %02x:%02x:%02x:%02x:%02x:%02x  len=%zd  data=",
                   from.sll_addr[0], from.sll_addr[1], from.sll_addr[2],
                   from.sll_addr[3], from.sll_addr[4], from.sll_addr[5], n);
        } else
        {
            printf("From <unknown> len=%zd data=", n);
        }

        for (ssize_t i = 0; i < n; i++)
        {
            const unsigned char c = buf[i];
            putchar(c >= 32 && c <= 126 ? c : '.');
        }
        putchar('\n');
        fflush(stdout);
    }

    close(s);
    return 0;
}
