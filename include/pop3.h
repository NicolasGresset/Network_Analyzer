#pragma once
#include "options.h"

#define POP3_SERVER_PORT 110

int pop3_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);