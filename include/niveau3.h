#pragma once
#include "options.h"
#include "niveau4.h"
#include <net/ethernet.h>

enum niveau3_type{
    IPV4 = ETHERTYPE_IP,
    IPV6 = ETHERTYPE_IPV6,
    ARP = ETHERTYPE_ARP,
    INCONNU_NIVEAU3 = 0x0000
} ;


/**
 * @brief Aiguille vers le handler correspondant au type de la trame de niveau 3
 * ou inconnu si le type n'est pas pris en charge par mon analyseur
 * Length correspond à la longueur restante à analyser :
 * longueur de la trame - longueur de l'entête ethernet
 * 
 * @param opt 
 * @param packet 
 * @param type 
 * @param length 
 * @return int 
 */
int niveau3_handler(options* opt, const char *packet, enum niveau3_type type, uint32_t length);


enum niveau4_type type_niveau4(uint8_t protocol);