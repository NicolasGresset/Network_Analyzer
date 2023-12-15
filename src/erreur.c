#include "../include/erreur.h"
#include "../include/capture.h"
#include <stdlib.h>

void raler(const char * msg){
    pcap_perror(capture, msg);
    exit(1);
}