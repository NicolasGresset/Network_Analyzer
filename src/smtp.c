#include "../include/smtp.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include <stdlib.h>

char * parse_smtp_command(const char *packet, uint32_t length){
    char *command = malloc(sizeof(char) * length);
    uint32_t i = 0;
    while(packet[i] != ' ' && i < length){
        command[i] = packet[i];
        i++;
    }
    command[i] = '\0';
    return command;
}

int smtp_handler(options *opt, const char *packet, uint16_t src_port,
                 uint16_t dst_port, uint32_t length) {
    char *command = parse_smtp_command(packet, length);
    char *payload = get_payload(packet, length);
    switch(opt->verbose){
        case CONCIS:
            printf("SMTP\n");
            break;
        case SYNTHETIQUE:
            printf("├─ SMTP, command \"%s\" from %s\n", command, get_sender(src_port, dst_port, SMTP_SERVER_PORT));
            break;
        case COMPLET:
            printf("├─ SMTP\n");
            printf("│  ├─ Payload : \n");
            display_with_prefix(payload, PREFIX);
            break;
    }
    free(command);
    free(payload);
    return 0;
}