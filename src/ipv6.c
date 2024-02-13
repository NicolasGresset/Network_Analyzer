#include "../include/ipv6.h"
#include "../include/niveau4.h"
#include "../include/erreur.h"
#include "../include/niveau3.h"
#include <netinet/ip6.h>

#define IPV6_STR_LENGTH 40

int ipv6_handler(options *opt, const char *packet, u_int32_t length) {
  struct ip6_hdr *ip6_header = (struct ip6_hdr *)packet;

  char adress_src[IPV6_STR_LENGTH];
  char adress_dst[IPV6_STR_LENGTH];
  NCHK(inet_ntop(AF_INET6, &ip6_header->ip6_src, adress_src, IPV6_STR_LENGTH));
  NCHK(inet_ntop(AF_INET6, &ip6_header->ip6_dst, adress_dst, IPV6_STR_LENGTH));

  switch (opt->verbose) {
  case CONCIS:
    printf("IPv6, ");
    break;
  case SYNTHETIQUE:
    printf("├─ IPv6, Src: %s, Dst: %s\n", adress_src, adress_dst);
    break;
  case COMPLET:
    printf("├─ IPv6\n");
    printf("│  ├─ Version: %u, Traffic class: 0x%02x, Flow label: 0x%05x\n",
           ip6_header->ip6_vfc >> 4, ip6_header->ip6_vfc & 0x0F,
           ntohl(ip6_header->ip6_flow));
    printf("│  ├─ Payload length: %u bytes, Next header: %u, Hop limit: %u\n",
           ntohs(ip6_header->ip6_plen), ip6_header->ip6_nxt,
           ip6_header->ip6_hlim);
    printf("│  ├─ Source: %s, Destination: %s\n", adress_src, adress_dst);
    break;
  }

  char *level4_packet = (char *)packet + sizeof(struct ip6_hdr);

  enum niveau4_type type = type_niveau4(ip6_header->ip6_nxt);
  return niveau4_handler(opt, level4_packet, type, length - sizeof(struct ip6_hdr));
}