#include "../include/niveau3.h"
#include "../include/arp.h"
#include "../include/erreur.h"
#include "../include/handler.h" // pour PROTOCOLE_INCONNU
#include "../include/ipv4.h"
#include "../include/ipv6.h"
#include "../include/niveau4.h"

/**
 * @brief Retourne le type de la trame de niveau 4 ou inconnu si le type n'est
 * pas pris en charge par mon analyseur
 *
 * @param protocol
 * @return enum niveau4_type
 */
enum niveau4_type type_niveau4(uint8_t protocol) {
  switch (protocol) {
  case IPPROTO_ICMP:
    return ICMP;
  case IPPROTO_TCP:
    return TCP;
  case IPPROTO_UDP:
    return UDP;
  default:
    return INCONNU_NIVEAU4;
  }
}

int inconnu_niveau3_handler(options *opt, const char *packet, uint32_t length) {
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

/**
 * @brief Appelle le handler correspondant au type de la trame de niveau 3 ou
 * inconnu si le type n'est pas pris en charge par mon analyseur
 *
 * @param opt
 * @param packet
 * @param type
 * @return int
 */
int niveau3_handler(options *opt, const char *packet, enum niveau3_type type, uint32_t length) {
  if (length == 0) {
    return 0;
  }
  switch (type) {
  case IPV4:
    return ipv4_handler(opt, packet, length);
  case IPV6:
    return ipv6_handler(opt, packet, length);
  case ARP:
    return arp_handler(opt, packet, length);
  default:
    return inconnu_niveau3_handler(opt, packet, length);
  }
}