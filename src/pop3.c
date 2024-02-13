#include "../include/pop3.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include <stdlib.h>

char *parse_pop3_command(const char *packet, uint32_t length) {
  char *command = malloc(sizeof(char) * length);
  uint32_t i = 0;
  while (packet[i] != ' ' && i < length) {
    command[i] = packet[i];
    i++;
  }
  command[i] = '\0';
  return command;
}

int pop3_handler(options *opt, const char *packet, uint16_t src_port,
                 uint16_t dst_port, uint32_t length) {
  char *command = parse_pop3_command(packet, length);
  char *payload = get_payload(packet, length);
  
  switch (opt->verbose) {
  case CONCIS:
    printf("POP3\n");
    break;
  case SYNTHETIQUE:
    printf("├─ POP3, command \"%s\" from %s\n", command,
           get_sender(src_port, dst_port, POP3_SERVER_PORT));
    break;
  case COMPLET:
    printf("├─ POP3\n");
    printf("│  ├─ Payload : \n");
    display_with_prefix(payload, PREFIX);
    break;
  }
  free(command);
  free(payload);
  return 0;
}