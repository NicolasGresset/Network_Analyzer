#include "../include/imap.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include <stdlib.h>

char *parse_imap_command(const char *packet, uint32_t length) {
  char *command = malloc(sizeof(char) * length);

  uint32_t offset = 0;
  while (packet[offset] != ' ' && offset < length) {
    offset++;
  }
  offset++;

  int i = 0;
  while (packet[offset] != ' ' && offset < length) {
    command[i] = packet[offset];
    offset++;
    i++;
  }
  command[i] = '\0';
  return command;
}

int imap_handler(options *opt, const char *packet, uint16_t src_port,
                 uint16_t dst_port, uint32_t length) {
  char *command = parse_imap_command(packet, length);
  char *payload = get_payload(packet, length);
  switch (opt->verbose) {
  case CONCIS:
    printf("IMAP\n");
    break;
  case SYNTHETIQUE:
    printf("├─ IMAP, command \"%s\" from %s\n", command,
           get_sender(src_port, dst_port, IMAP_SERVER_PORT));
    break;
  case COMPLET:
    printf("├─ IMAP\n");
    printf("│  ├─ Payload : \n");
    display_with_prefix(payload, PREFIX);
    break;
  }
  free(command);
  free(payload);
  return 0;
}