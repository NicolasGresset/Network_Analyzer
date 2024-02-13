#include "../include/niveau4.h"
#include "../include/tcp.h"
#include "../include/icmp.h"
#include "../include/udp.h"

int inconnu_niveau4_handler(options *opt, char *packet, uint32_t length) {
  (void)packet;
  (void)length;
  switch (opt->verbose) {
  case CONCIS:
    printf("Inconnu\n");
    break;
  case SYNTHETIQUE:
    printf("├─ Inconnu\n");
    break;
  case COMPLET:
    printf("├─ Inconnu\n");
    break;
  }
  return 0;
}

int niveau4_handler(options *opt, char *packet, enum niveau4_type type, uint32_t length) {
  if (length == 0) {
    return 0;
  }
  switch (type) {
  case ICMP:
    return icmp_handler(opt, packet, length);
  case TCP:
    return tcp_handler(opt, packet, length);
  case UDP:
    return udp_handler(opt, packet, length);
  default:
    return inconnu_niveau4_handler(opt, packet, length);
  }
}