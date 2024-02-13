#pragma once
#include "options.h"

#define BOOTP_REQUEST_PORT 67
#define BOOTP_REPLY_PORT 68

int bootp_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);