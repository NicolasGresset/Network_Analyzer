#include "../include/http.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include <stdlib.h>

/**
 * @brief Renvoie la première ligne de la réponse HTTP
 *
 * @param packet
 * @param length
 * @return char*
 */
char *parse_http_command(const char *packet, uint32_t length) {
  char *command = malloc(sizeof(char) * length + 1);
  uint32_t i = 0;
  while (packet[i] != '\n' && i < length) {
    command[i] = packet[i];
    i++;
  }
  command[i] = '\0';
  return command;
}

char * request_or_response(uint16_t src_port, uint16_t dst_port) {
  if (src_port == HTTP_SERVER_PORT) {
    return "Response";
  } else if (dst_port == HTTP_SERVER_PORT) {
    return "Request";
  }
  return "unknown";
}

int http_handler(options *opt, const char *packet, uint16_t src_port,
                 uint16_t dst_port, uint32_t length) {
  char *payload = get_payload(packet, length);
  remove_carriage_return(payload);
  char *command = parse_http_command(payload, length + 1);

  switch (opt->verbose) {
  case CONCIS:
    printf("HTTP\n");
    break;
  case SYNTHETIQUE:
    printf("├─ HTTP, command \"%s\" from %s\n", command,
           get_sender(src_port, dst_port, HTTP_SERVER_PORT));
    break;
  case COMPLET:
    printf("├─ HTTP\n");
    printf("│  ├─ %s, payload : \n", request_or_response(src_port, dst_port));
    display_with_prefix(payload, PREFIX);
    break;
  }
  free(command);
  free(payload);
  return 0;
}