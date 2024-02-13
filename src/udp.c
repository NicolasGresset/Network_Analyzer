#include "../include/udp.h"
#include "../include/applicatif.h"
#include <netinet/udp.h>

int udp_handler(options *opt, char *packet, uint32_t length) {
  struct udphdr *udp_header = (struct udphdr *)packet;

  switch (opt->verbose) {
  case CONCIS:
    printf("UDP, ");
    break;
  case SYNTHETIQUE:
    printf("├─ UDP, Src port: %u, Dst port: %u\n", ntohs(udp_header->source),
           ntohs(udp_header->dest));
    break;
  case COMPLET:
    printf("├─ UDP\n");
    printf("│  ├─ Src port: %u\n", ntohs(udp_header->source));
    printf("│  ├─ Dst port: %u\n", ntohs(udp_header->dest));
    printf("│  ├─ Length: %u bytes\n", ntohs(udp_header->len));
    printf("│  ├─ Checksum: 0x%04x\n", ntohs(udp_header->check));
    break;
  }
  return applicatif_handler(opt, packet + sizeof(struct udphdr),
                            ntohs(udp_header->source), ntohs(udp_header->dest),
                            length - sizeof(struct udphdr));
}