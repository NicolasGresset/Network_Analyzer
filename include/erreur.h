#pragma once
#include <pcap.h>

#define PCHK(op)                                                                \
    do {                                                                       \
        if ((op) == PCAP_ERROR)                                                        \
            raler(#op);                                                     \
    } while (0)


/**
 * @brief Display error with perror then exit
 * 
 * @param capture 
 * @param msg 
 */
void raler(const char * msg);