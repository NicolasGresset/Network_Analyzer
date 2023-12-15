#pragma once

#include "options.h"

enum niveau4_type{
    ICMP = 0x01,
    TCP = 0x06,
    UDP = 0x11,
    INCONNU_NIVEAU4 = 0x00
};


int niveau4_handler(options* opt, char *packet, enum niveau4_type type);