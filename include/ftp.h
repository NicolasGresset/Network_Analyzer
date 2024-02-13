#pragma once
#include "options.h"

#define FTP_SERVER_CONTROL_PORT 21
#define FTP_SERVER_DATA_PORT 20

int ftp_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length);