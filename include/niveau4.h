#pragma once

#include "options.h"

enum niveau4_type{
    ICMP = 0x01,
    TCP = 0x06,
    UDP = 0x11,
    INCONNU_NIVEAU4 = 0x00
};

/**
 * @brief Aiguille vers le handler correspondant au type de la trame de niveau 4
 * ou inconnu si le type n'est pas pris en charge par mon analyseur
 * Length correspond à la longueur restante à analyser :
 * longueur de la trame - longueur de l'entête ethernet - longueur de l'entête niveau 3
 * 
 * @param opt 
 * @param packet 
 * @param type 
 * @param length 
 * @return int 
 */
int niveau4_handler(options* opt, char *packet, enum niveau4_type type, u_int32_t length);