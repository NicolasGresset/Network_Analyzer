#pragma once
#include <net/ethernet.h>
#include "options.h"
#include "niveau3.h"


char *ether_ntoa(uint8_t addr[ETH_ALEN]);

/**
 * @brief Affiche les options concernant la trame packet conformément
 * aux options opt, puis appelle niveau3_handler en fonction du type de trame.
 * Length est la longueur de la trame.
 * 
 * @param opt 
 * @param packet 
 * @param length
 * @return void
 */
void niveau2_handler(options* opt,const char* packet, uint32_t length);
