#include "../include/ftp.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include <stdlib.h>

char *parse_ftp_command(const char *packet, uint32_t length) {
  char *command = malloc(sizeof(char) * length + 1);
  uint32_t i = 0;
  while (packet[i] != ' ' && i < length) {
    command[i] = packet[i];
    i++;
  }
  command[i] = '\0';
  return command;
}

int ftp_handler(options *opt, const char *packet, uint16_t src_port,
                uint16_t dst_port, uint32_t length) {
  char *payload = get_payload(packet, length);
  char *command = parse_ftp_command(packet, length);
  switch (opt->verbose) {
  case CONCIS:
    printf("FTP\n");
    break;
  case SYNTHETIQUE:
    if (src_port == FTP_SERVER_CONTROL_PORT ||
        dst_port == FTP_SERVER_CONTROL_PORT) {
      printf("├─ FTP, control from %s : command \"%s\"\n",
             get_sender(src_port, dst_port, FTP_SERVER_CONTROL_PORT), command);
    } else if (src_port == FTP_SERVER_DATA_PORT ||
               dst_port == FTP_SERVER_DATA_PORT) {
      printf("├─ FTP, data from %s\n",
             get_sender(src_port, dst_port, FTP_SERVER_DATA_PORT));
    }
    break;
  case COMPLET:
    printf("├─ FTP\n");
    printf("│  ├─ Payload : \n");
    display_with_prefix(payload, PREFIX);
    break;
  }
  free(payload);
    free(command);
  return 0;
}