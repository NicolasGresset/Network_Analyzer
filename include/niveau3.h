#pragma once
#include "../include/options.h"
#include <net/ethernet.h>

enum niveau3_type{
    IP = ETHERTYPE_IP,
    ARP = ETHERTYPE_ARP,
    RARP = ETHERTYPE_REVARP,
    INCONNU_NIVEAU3 = 0x0000
} ;


int niveau3_handler(options* opt, const char *packet, enum niveau3_type type);


