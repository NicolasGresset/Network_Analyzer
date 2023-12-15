#include "../include/niveau3.h"
#include "../include/handler.h" // pour PROTOCOLE_INCONNU
#include "../include/niveau4.h"
#include <netinet/in.h>
#include <netinet/ip.h>

#define IPV4 4
#define IPV6 6



char *inet_ntoa_r(uint32_t addr) {
  struct in_addr in_addr = {addr};
  return inet_ntoa(in_addr);
}

void print_ipv4_options(const struct iphdr *packet) {
  // todo
}

int ipv4_handler(options *opt, const char *packet) {
  const struct iphdr *ip_header = (const struct iphdr*)packet;
  if (ip_header->ihl < 5 || ip_header->ihl * 4 > ntohs(ip_header->tot_len)) {
    printf("   * Invalid IP header length: %u bytes\n", ip_header->ihl * 4);
    return PROTOCOLE_INCONNU;
  }

  switch (opt->verbose) {
  case CONCIS:
    printf("IPv4, ");
    break;
  case SYNTHETIQUE:
    printf("IPv4, Src: %s, Dst: %s\n", inet_ntoa_r(ip_header->saddr),
           inet_ntoa_r(ip_header->daddr));
    break;
  case COMPLET:
    printf("Version: %u, IHL: %u, TOS: 0x%02x, Total Length: %u, ",
           ip_header->version, ip_header->ihl * 4, ip_header->tos,
           ntohs(ip_header->tot_len));
    printf("Identification: 0x%04x, Flags: 0x%02x, Fragment Offset: %u, ",
           ntohs(ip_header->id), ip_header->frag_off & 0xe000,
           ip_header->frag_off & 0x1fff);
    printf("TTL: %u, Protocol: %u, Header Checksum: 0x%04x\n", ip_header->ttl,
           ip_header->protocol, ntohs(ip_header->check));
    printf("Source: %s\n", inet_ntoa_r(ip_header->saddr));
    printf("Destination: %s\n", inet_ntoa_r(ip_header->daddr));
    print_ipv4_options(ip_header);
    break;
  }

  char *level4_packet = (char *)packet + ip_header->ihl * 4;

  enum niveau4_type type;
  switch (ip_header->protocol) {
  case IPPROTO_ICMP:
    type = ICMP;
    break;
  case IPPROTO_TCP:
    type = TCP;
    break;
  case IPPROTO_UDP:
    type = UDP;
    break;
  default:
    type = INCONNU_NIVEAU4;
    break;
  }
  return niveau4_handler(opt, level4_packet, type);
}



int ipv6_handler(options *opt, const char *packet){}

int ip_handler(options *opt, const char *packet) {
  const struct iphdr *ip_header = (const struct iphdr*)packet;
  switch (ip_header->version) {
  case IPV4:
    return ipv4_handler(opt, packet);
  case IPV6:
    return ipv6_handler(opt, packet);
  default:
    return PROTOCOLE_INCONNU;
  }
}

int arp_handler(options *opt, const char *packet) {}

int rarp_handler(options *opt, const char *packet) {}

int inconnu_niveau3_handler(options *opt, const char *packet) {
  printf("protocole de niveau 3 inconnu (différent de ip, arp et rarp)\n");
  return PROTOCOLE_INCONNU;
}



int niveau3_handler(options *opt, const char *packet, enum niveau3_type type) {
  switch (type) {
  case IP:
    return ip_handler(opt, packet);
  case ARP:
    return arp_handler(opt, packet);
  case RARP:
    return rarp_handler(opt, packet);
  default:
    return inconnu_niveau3_handler(opt, packet);
  }
}