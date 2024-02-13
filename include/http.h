#pragma once
#include "options.h"

#define HTTP_SERVER_PORT 80

int http_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);