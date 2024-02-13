#pragma once
#include "options.h"

#define SMTP_SERVER_PORT 25

int smtp_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);