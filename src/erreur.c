#include "../include/erreur.h"
#include "../include/capture.h"
#include <stdlib.h>

void pcap_raler(const char * msg){
    pcap_perror(capture, msg);
    exit(1);
}

void raler(const char *msg){
    perror(msg);
    exit(1);
}