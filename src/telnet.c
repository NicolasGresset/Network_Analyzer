#include "../include/telnet.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include "../include/erreur.h"
#include <stdlib.h>
#include <string.h>

#define OPTION_LENGTH 16

int parse_telnet_message(const char *message, uint32_t length,
                         uint32_t offset) {

  if (offset >= length) {
    return 0;
  } else if (message[offset] == '\xff' && offset + 1 < length) {
    char *option;
    NCHK(option = malloc(sizeof(char) * OPTION_LENGTH));
    switch (message[offset + 1]) {
    case '\xf0':
      snprintf(option, OPTION_LENGTH, "SE\n");
      break;
    case '\xf1':
      snprintf(option, OPTION_LENGTH, "NOP\n");
      break;
    case '\xf2':
      snprintf(option, OPTION_LENGTH, "DM\n");
      break;
    case '\xf3':
      snprintf(option, OPTION_LENGTH, "BRK\n");
      break;
    case '\xf4':
      snprintf(option, OPTION_LENGTH, "IP\n");
      break;
    case '\xf5':
      snprintf(option, OPTION_LENGTH, "AO\n");
      break;
    case '\xf6':
      snprintf(option, OPTION_LENGTH, "AYT\n");
      break;
    case '\xf7':
      snprintf(option, OPTION_LENGTH, "EC\n");
      break;
    default:
      snprintf(option, OPTION_LENGTH, "UNKNOWN\n");
      break;
    }
    display_with_prefix(option, PREFIX);
    free(option);
    return parse_telnet_message(message, length, offset + 2);
  } else if (message[offset] == '\xff' && offset + 1 == length) {
    display_with_prefix("IAC\n", PREFIX);
    return 0;
  } else {
    uint32_t i = offset;
    while (message[i] != '\xff' && i < length) {
      i++;
    }
    char *sub_message = malloc(sizeof(char) * (i - offset + 1));
    memmove(sub_message, message + offset, i - offset);
    sub_message[i - offset] = '\0';
    display_with_prefix(sub_message, PREFIX);
    free(sub_message);
    return parse_telnet_message(message, length, i);
  }
}

int telnet_handler(options *opt, const char *packet, uint16_t src_port,
                   uint16_t dst_port, uint32_t length) {
  char *payload = get_payload(packet, length);

  switch (opt->verbose) {
  case CONCIS:
    printf("Telnet\n");
    break;
  case SYNTHETIQUE:
    printf("├─ Telnet, message from %s\n",
           get_sender(src_port, dst_port, TELNET_SERVER_PORT));
    break;
  case COMPLET:
    printf("├─ Telnet\n");
    printf("│  ├─ Payload : \n");
    parse_telnet_message(payload, length, 0);
    break;
  }
  free(payload);
  return 0;
}