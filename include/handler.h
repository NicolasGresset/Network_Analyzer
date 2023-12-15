#pragma once

#include <pcap.h>

#define CHK(op)                                                                \
    do {                                                                       \
        if ((op) == -1)                                                        \
            raler(1, #op);                                                     \
    } while (0)


#define PROTOCOLE_INCONNU -1

/**
 * @brief Le handler appele a chaque reception de paquet
 * Parse les arguments et les passe a niveau2_handler
 * 
 * @param args 
 * @param header 
 * @param packet 
 */
void handler(u_char *args, const struct pcap_pkthdr *header,
             const u_char *packet);