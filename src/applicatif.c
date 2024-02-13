#include "../include/applicatif.h"
#include "../include/affichage.h"
#include "../include/bootp.h"
#include "../include/ftp.h"
#include "../include/http.h"
#include "../include/imap.h"
#include "../include/pop3.h"
#include "../include/smtp.h"
#include "../include/telnet.h"
#include <stdlib.h>
#include <string.h>

char *get_sender(uint16_t src_port, uint16_t dst_port, uint16_t server_port) {
  if (src_port == server_port) {
    return "server";
  } else if (dst_port == server_port) {
    return "client";
  }
  return "unknown";
}

char *get_payload(const char *packet, uint32_t length) {
  char *payload = malloc(length + 1);
  memmove(payload, packet, length);
  payload[length] = '\0';
  return payload;
}

int inconnu_applicatif_handler(options *opt, const char *packet,
                               uint16_t src_port, uint16_t dst_port,
                               uint32_t length) {
  (void)packet;
  (void)src_port;
  (void)dst_port;
  (void)length;

  switch (opt->verbose) {
  case CONCIS:
    printf("Application inconnue\n");
    break;
  case SYNTHETIQUE:
    printf("├─ Application inconnue\n");
    break;
  case COMPLET:
    printf("├─ Application inconnue\n");
    printf("│  ├─ Payload : \n");
    display_with_prefix(packet, PREFIX);
    break;
  }
  return 0;
}

int applicatif_handler(options *opt, const char *packet, uint16_t src_port,
                       uint16_t dst_port, uint32_t length) {

  if (length == 0) {
    return 0;
  }
  if (src_port == SMTP_SERVER_PORT || dst_port == SMTP_SERVER_PORT) {
    return smtp_handler(opt, packet, src_port, dst_port, length);
  } else if (src_port == POP3_SERVER_PORT || dst_port == POP3_SERVER_PORT) {
    return pop3_handler(opt, packet, src_port, dst_port, length);
  } else if (src_port == IMAP_SERVER_PORT || dst_port == IMAP_SERVER_PORT) {
    return imap_handler(opt, packet, src_port, dst_port, length);
  } else if (src_port == HTTP_SERVER_PORT || dst_port == HTTP_SERVER_PORT) {
    return http_handler(opt, packet, src_port, dst_port, length);
  } else if (src_port == TELNET_SERVER_PORT || dst_port == TELNET_SERVER_PORT)
    return telnet_handler(opt, packet, src_port, dst_port, length);
  else if (src_port == FTP_SERVER_CONTROL_PORT ||
           dst_port == FTP_SERVER_CONTROL_PORT ||
           src_port == FTP_SERVER_DATA_PORT || dst_port == FTP_SERVER_DATA_PORT)
    return ftp_handler(opt, packet, src_port, dst_port, length);
  else if (src_port == BOOTP_REPLY_PORT || dst_port == BOOTP_REPLY_PORT ||
           src_port == BOOTP_REQUEST_PORT || dst_port == BOOTP_REQUEST_PORT)
    return bootp_handler(opt, packet, src_port, dst_port, length);
  else {
    return inconnu_applicatif_handler(opt, packet, src_port, dst_port, length);
  }
  return 0;
}