#pragma once
#include "options.h"


/**
 * @brief Aiguille le packet vers le bon handler en fonction du port de destination
 * Length est la taille du payload
 * 
 * @param opt 
 * @param packet 
 * @param src 
 * @param dst 
 * @param length 
 * @return int 
 */
int applicatif_handler(options* opt, const char *packet, uint16_t src, uint16_t dst, uint32_t length);


/**
 * @brief Renvoie "server" si le port source est le port du serveur,
 * "client" si le port destination est le port du serveur, et "unknown" sinon
 * 
 * @param src_port 
 * @param dst_port 
 * @param server_port 
 * @return char* 
 */
char *get_sender(uint16_t src_port, uint16_t dst_port, uint16_t server_port);


/**
 * @brief Renvoie une chaine de caractères contenant le payload du packet
 * La chaine de retour doit être libérée par l'appelant
 * 
 * @param packet 
 * @param length 
 * @return char* 
 */
char *get_payload(const char *packet, uint32_t length);