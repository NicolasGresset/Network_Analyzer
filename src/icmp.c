#include "../include/applicatif.h"
#include "../include/icmp.h"
#include <netinet/ip_icmp.h>

void manage_icmp_type_code(struct icmphdr *icmp_header) {
  switch (icmp_header->type) {
  case ICMP_ECHOREPLY:
    printf("├─ Type: Echo reply\n");
    break;
  case ICMP_DEST_UNREACH:
    printf("├─ Type: Destination unreachable\n");
    switch (icmp_header->code) {
    case ICMP_NET_UNREACH:
      printf("│  ├─ Code: Network unreachable\n");
      break;
    case ICMP_HOST_UNREACH:
      printf("│  ├─ Code: Host unreachable\n");
      break;
    case ICMP_PROT_UNREACH:
      printf("│  ├─ Code: Protocol unreachable\n");
      break;
    case ICMP_PORT_UNREACH:
      printf("│  ├─ Code: Port unreachable\n");
      break;
    case ICMP_FRAG_NEEDED:
      printf("│  ├─ Code: Fragmentation needed and DF set\n");
      break;
    case ICMP_SR_FAILED:
      printf("│  ├─ Code: Source route failed\n");
      break;
    case ICMP_NET_UNKNOWN:
      printf("│  ├─ Code: Network unknown\n");
      break;
    case ICMP_HOST_UNKNOWN:
      printf("│  ├─ Code: Host unknown\n");
      break;
    case ICMP_HOST_ISOLATED:
      printf("│  ├─ Code: Host isolated\n");
      break;
    case ICMP_NET_ANO:
      printf("│  ├─ Code: Network administratively prohibited\n");
      break;
    case ICMP_HOST_ANO:
      printf("│  ├─ Code: Host administratively prohibited\n");
      break;
    case ICMP_NET_UNR_TOS:
      printf("│  ├─ Code: Network unreachable for TOS\n");
      break;
    case ICMP_HOST_UNR_TOS:
      printf("│  ├─ Code: Host unreachable for TOS\n");
      break;
    case ICMP_PKT_FILTERED:
      printf("│  ├─ Code: Packet filtered\n");
      break;
    case ICMP_PREC_VIOLATION:
      printf("│  ├─ Code: Precedence violation\n");
      break;
    case ICMP_PREC_CUTOFF:
      printf("│  ├─ Code: Precedence cutoff\n");
      break;
    default:
      printf("│  ├─ Code: Unknown\n");
      break;
    }
    break;
  case ICMP_SOURCE_QUENCH:
    printf("├─ Type: Source quench\n");
    break;
  case ICMP_REDIRECT:
    printf("├─ Type: Redirect\n");
    switch (icmp_header->code) {
    case ICMP_REDIR_NET:
      printf("│  ├─ Code: Redirect datagrams for the Network\n");
      break;
    case ICMP_REDIR_HOST:
      printf("│  ├─ Code: Redirect datagrams for the Host\n");
      break;
    case ICMP_REDIR_NETTOS:
      printf("│  ├─ Code: Redirect datagrams for the Type of Service and "
             "Network\n");
      break;
    case ICMP_REDIR_HOSTTOS:
      printf(
          "│  ├─ Code: Redirect datagrams for the Type of Service and Host\n");
      break;
    default:
      printf("│  ├─ Code: Unknown\n");
      break;
    }
    break;
  case ICMP_ECHO:
    printf("├─ Type: Echo request\n");
    break;
  case ICMP_TIME_EXCEEDED:
    printf("├─ Type: Time exceeded\n");
    switch (icmp_header->code) {
    case ICMP_EXC_TTL:
      printf("│  ├─ Code: Time to live exceeded in transit\n");
      break;
    case ICMP_EXC_FRAGTIME:
      printf("│  ├─ Code: Fragment reassembly time exceeded\n");
      break;
    default:
      printf("│  ├─ Code: Unknown\n");
      break;
    }
    break;
  default:
    printf("├─ Type: Unknown\n");
    break;
  }
}

int icmp_handler(options *opt, char *packet, uint32_t length) {
  (void)length;
  struct icmphdr *icmp_header = (struct icmphdr *)packet;

  switch (opt->verbose) {
  case CONCIS:
    printf("ICMP, ");
    break;
  case SYNTHETIQUE:
    printf("├─ ICMP, Type: %u, Code: %u\n", icmp_header->type,
           icmp_header->code);
    break;
  case COMPLET:
    printf("├─ ICMP\n");
    manage_icmp_type_code(icmp_header);
    printf("│  ├─ Checksum: 0x%04x\n", ntohs(icmp_header->checksum));
    printf("│  ├─ Identifier: %u\n", ntohs(icmp_header->un.echo.id));
    printf("│  ├─ Sequence number: %u\n", ntohs(icmp_header->un.echo.sequence));
    break;
  }
  return 0;
}