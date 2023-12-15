#pragma once
#include <net/ethernet.h>
#include "options.h"
#include "niveau3.h"


/**
 * @brief Affiche les options concernant la trame packet conformément
 * aux options opt, puis appelle niveau3_handler en fonction du type de trame.
 * 
 * @param opt 
 * @param packet 
 * @return v
 */
int niveau2_handler(options* opt,const char* packet);
