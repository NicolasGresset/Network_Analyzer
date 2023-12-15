#pragma once
#include <pcap.h>
#include "options.h"


extern pcap_t *capture;

/**
 * @brief Commence la capture conformément aux options opt et
 * renvoie un pointeur vers le descripteur de capture correspondant
 * 
 * @param opt 
 * @return pcap_t* 
 */
pcap_t *open_capture(options *opt);

void start_loop(pcap_t * capture, pcap_handler handler, options * opt);

void manage_filter(pcap_t* capture, options* opt);