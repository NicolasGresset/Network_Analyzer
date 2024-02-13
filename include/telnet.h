#pragma once
#include "options.h"

#define TELNET_SERVER_PORT 23

int telnet_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);