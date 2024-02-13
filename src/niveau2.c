#include "../include/niveau2.h"
#include <stdio.h>
#include <stdlib.h>

#define MAC_STRING_LENGTH 18
#define TYPE_STRING_LENGTH 32

char *ether_ntoa(uint8_t addr[ETH_ALEN]) {
  char *ethernet_address_readable = malloc(MAC_STRING_LENGTH);
  for (int i = 0; i < ETH_ALEN; i++) {
    sprintf(ethernet_address_readable + i * 3, "%02hhx", addr[i]);
    if (i != ETH_ALEN - 1) {
      sprintf(ethernet_address_readable + i * 3 + 2, ":");
    }
  }
  ethernet_address_readable[MAC_STRING_LENGTH - 1] = '\0';
  return ethernet_address_readable;
}

/**
 * @brief Renvoie le type de la trame ethernet ou inconnu si le type n'est pas
 * pris en charge par l'analyseur
 *
 * @param ethertype
 * @return enum niveau3_type
 */
enum niveau3_type niveau3_type_from_ethertype(uint16_t ethertype) {
  switch (ethertype) {
  case ETHERTYPE_IP:
    return IPV4;
  case ETHERTYPE_IPV6:
    return IPV6;
  case ETHERTYPE_ARP:
    return ARP;
  case ETHERTYPE_REVARP:
    return ARP;
  default:
    return INCONNU_NIVEAU3;
  }
}

char *ether_type(uint16_t ethertype) {
  char *type = malloc(TYPE_STRING_LENGTH);

  switch (ethertype) {
  case ETHERTYPE_IP:
    snprintf(type, TYPE_STRING_LENGTH, "0x%04x (IPv4)", ethertype);
    break;
  case ETHERTYPE_IPV6:
    snprintf(type, TYPE_STRING_LENGTH, "0x%04x (IPv6)", ethertype);
    break;
  case ETHERTYPE_ARP:
    snprintf(type, TYPE_STRING_LENGTH, "0x%04x (ARP)", ethertype);
    break;
  case ETHERTYPE_REVARP:
    snprintf(type, TYPE_STRING_LENGTH, "0x%04x (RARP)", ethertype);
    break;
  default:
    snprintf(type, TYPE_STRING_LENGTH, "0x%04x (inconnu)", ethertype);
    break;
  }
  return type;
}

void niveau2_handler(options *opt, const char *packet, uint32_t length) {
  struct ether_header *ethernet_header = (struct ether_header *)packet;
  char *adress_src = ether_ntoa(ethernet_header->ether_shost);
  char *adress_dst = ether_ntoa(ethernet_header->ether_dhost);
  char *type_string = ether_type(ntohs(ethernet_header->ether_type));

  printf("\n");
  switch (opt->verbose) {
  case CONCIS:
    printf("├─ Ethernet, ");
    break;
  case SYNTHETIQUE:
    printf("├─ Ethernet, Src: %s, Dst: %s\n", adress_src, adress_dst);
    break;
  case COMPLET:
    printf("├─ Ethernet\n");
    printf("│  ├─ Source: %s, Destination : %s\n", adress_src, adress_dst);
    printf("│  ├─ Type: %s\n", type_string);
    break;
  }

  free(adress_src);
  free(adress_dst);
  free(type_string);

  enum niveau3_type type =
      niveau3_type_from_ethertype(ntohs(ethernet_header->ether_type));
  niveau3_handler(opt, packet + sizeof(struct ether_header), type, length - sizeof(struct ether_header));
}