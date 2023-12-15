#include "../include/niveau2.h"
#include <stdlib.h>
#include <stdio.h>


char *ether_ntoa(uint8_t addr[ETH_ALEN]) {
    char *ethernet_address_readable = malloc(ETH_ALEN*2 + ETH_ALEN);
    for (int i = 0; i < ETH_ALEN; i++) {
        sprintf(ethernet_address_readable + i*3, "%02hhx", addr[i]);
        if (i != ETH_ALEN - 1) {
            sprintf(ethernet_address_readable + i*3 + 2, ":");
        }
    }
    ethernet_address_readable[ETH_ALEN*2 + ETH_ALEN - 1] = '\0';
    return ethernet_address_readable;
}

int niveau2_handler(options* opt, const char* packet){
    struct ether_header *ethernet_header = (struct ether_header *)packet;
    switch(opt->verbose){
        case CONCIS:
            printf("Ethernet, ");
            break;
        case SYNTHETIQUE:
            printf("Ethernet II, Src: %s, Dst: %s\n", ether_ntoa(ethernet_header->ether_shost), ether_ntoa(ethernet_header->ether_dhost));
            break;
        case COMPLET:
            printf("Src : %hhn, Destination : %hhn\n", ethernet_header->ether_dhost, ethernet_header->ether_shost);
            // printf("Ethernet II, Src: %s, Dst: %s, ", ether_ntoa(ethernet_header->ether_shost), ether_ntoa(ethernet_header->ether_dhost));
            printf("DataType = %d\n", ethernet_header->ether_type);
            break;
    }
    
    enum niveau3_type type;
    switch(ntohs(ethernet_header->ether_type)){
        case IP:
            type = IP;
            break;
        case ARP:
            type = ARP;
            break;
        case RARP:
            type = RARP;
            break;
        default:
            type = INCONNU_NIVEAU3;
            break;
    }
    return niveau3_handler(opt, packet + sizeof(struct ether_header), type);
}