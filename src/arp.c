#include "../include/arp.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include "../include/niveau2.h"
#include <linux/if_ether.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdlib.h>

char *get_arp_operation(struct arphdr *arp_header) {
  switch (ntohs(arp_header->ar_op)) {
  case ARPOP_REQUEST:
    return "Request";
    break;
  case ARPOP_REPLY:
    return "Reply";
    break;
  case ARPOP_RREQUEST:
    return "Reverse Request";
    break;
  case ARPOP_RREPLY:
    return "Reverse Reply";
    break;
  case ARPOP_InREQUEST:
    return "InARP Request";
    break;
  case ARPOP_InREPLY:
    return "InARP Reply";
    break;
  case ARPOP_NAK:
    return "ARP NAK";
    break;
  default:
    return "Unknown";
    break;
  }
}

void print_arp_hardware_type(struct arphdr *arp_header) {
  unsigned short ar_hrd = ntohs(arp_header->ar_hrd);
  switch (ar_hrd) {
  case ARPHRD_NETROM:
    printf("│  ├─ Hardware type: %hu (NET/ROM pseudo)\n", ar_hrd);
    break;
  case ARPHRD_ETHER:
    printf("│  ├─ Hardware type: %hu (Ethernet 10/100Mbps)\n", ar_hrd);
    break;
  case ARPHRD_EETHER:
    printf("│  ├─ Hardware type: %hu (Experimental Ethernet)\n", ar_hrd);
    break;
  default:
    printf("│  ├─ Hardware type: %hu (Unknown)\n", ar_hrd);
    break;
  }
}

void print_arp_protocol_type(struct arphdr *arp_header) {
  unsigned short ar_pro = ntohs(arp_header->ar_pro);
  switch (ar_pro) {
  case ETH_P_IP:
    printf("│  ├─ Protocol type: 0x%04x (IPv4)\n", ar_pro);
    break;
  case ETH_P_IPV6:
    printf("│  ├─ Protocol type: 0x%04x (IPv6)\n", ar_pro);
    break;
  default:
    printf("│  ├─ Protocol type: 0x%04x (Unknown)", ar_pro);
    break;
  }
}

int arp_handler(options *opt, const char *packet, u_int32_t length) {
  (void)length;
  struct arphdr *arp_header = (struct arphdr *)packet;
  char *start_adresses = (char *)packet + sizeof(struct arphdr);
  
  char *sender_hardware_adress = get_payload(start_adresses, arp_header->ar_hln);
  char *sender_logical_adress =
      get_payload(start_adresses + arp_header->ar_hln, arp_header->ar_pln);
  char *target_hardware_adress = get_payload(
      start_adresses + arp_header->ar_hln + arp_header->ar_pln, arp_header->ar_hln);
  char *target_logical_adress = get_payload(
      start_adresses + 2 * arp_header->ar_hln + arp_header->ar_pln, arp_header->ar_pln);

  switch (opt->verbose) {
  case CONCIS:
    printf("ARP, ");
    break;
  case SYNTHETIQUE:
    printf("├─ ARP, Operation : %s, ", get_arp_operation(arp_header));
    break;
  case COMPLET:
    printf("├─ ARP\n");
    print_arp_hardware_type(arp_header);
    print_arp_protocol_type(arp_header);
    printf("│  ├─ Operation : %s\n", get_arp_operation(arp_header));
    printf("│  ├─ Hardware size: %hu, Logical adress size: %hu\n",
           arp_header->ar_hln, arp_header->ar_pln);
    printf("│  ├─ Opcode: %hu\n", ntohs(arp_header->ar_op));
    printf("│  ├─ Sender hardware adress: 0x");
    display_as_hexa(sender_hardware_adress, arp_header->ar_hln);
    printf("\n");
    printf("│  ├─ Sender logical adress: 0x");
    display_as_hexa(sender_logical_adress, arp_header->ar_pln);
    printf("\n");
    printf("│  ├─ Target hardware adress: 0x");
    display_as_hexa(target_hardware_adress, arp_header->ar_hln);
    printf("\n");
    printf("│  ├─ Target logical adress: 0x");
    display_as_hexa(target_logical_adress, arp_header->ar_pln);
    printf("\n");
    break;
  }
  printf("\n");
  free(sender_hardware_adress);
  free(sender_logical_adress);
  free(target_hardware_adress);
  free(target_logical_adress);
  return 1;
}