#pragma once
#include "options.h"

#define IMAP_SERVER_PORT 143

int imap_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);