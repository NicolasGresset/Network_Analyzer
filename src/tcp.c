#include "../include/tcp.h"
#include "../include/applicatif.h"
#include <netinet/tcp.h>

#define END_OF_OPTIONS 0
#define NO_OPERATION 1

union tcp_option {
  uint8_t kind;
  struct {
    uint8_t kind;
    uint8_t length;
    char data[];
  } tlv;
};

void print_tcp_option(union tcp_option *option) {
  switch (option->kind) {
  case END_OF_OPTIONS:
    printf("│  ├─ Option: End of options list\n");
    break;
  case NO_OPERATION:
    printf("│  ├─ Option: No operation\n");
    break;
  case TCP_MAXSEG:
    printf("│  ├─ Option: Maximum segment size\n");
    printf("│  │  ├─ Kind: %u\n", option->tlv.kind);
    printf("│  │  ├─ Length: %u\n", option->tlv.length);
    printf("│  │  ├─ MSS value: %u\n", ntohs(*(uint16_t *)option->tlv.data));
    break;
  default:
    printf("│  ├─ Option: Unknown, the remaining options are ignored\n");
    printf("│  │  ├─ Kind: %u\n", option->kind);
    break;
  }
}

union tcp_option *next_tcp_option(union tcp_option *option,
                                  uint8_t header_length) {
  switch (option->kind) {
  case END_OF_OPTIONS:
    return option;
  case NO_OPERATION:
    return (union tcp_option *)((char *)option + 1);
  case TCP_MAXSEG:
    return (union tcp_option *)((char *)option + option->tlv.length);
  default:
    // si on ne reconnaît pas l'option, on ne peut pas parser la suite des
    // options
    // en effet, on ne peut pas savoir si le kind correspond à une
    // option seule ou à une option de type TLV : on ne peut donc pas savoir où
    // se trouve la prochaine option
    //  on va donc ignorer la suite des options TCP pour ne pas raconter de
    //  bétises
    // un analyseur plus complet devrait prendre
    // en charge toutes les options

    // en renvoyant option + header_length, on est sur de faire tomber la
    // condition du for dans print_tcp_options
    return (union tcp_option *)((char *)option + header_length);
  }
}

int is_end_tcp_options(union tcp_option *option, struct tcphdr *tcp_header) {
  return (((char *)option >= (char *)tcp_header + 4 * tcp_header->doff) ||
          (option->kind == END_OF_OPTIONS));
}

void print_tcp_options(struct tcphdr *tcp_header) {
  char *start_options = (char *)tcp_header + sizeof(struct tcphdr);

  union tcp_option *option;
  for (option = (union tcp_option *)start_options;
       !is_end_tcp_options(option, tcp_header);
       option = next_tcp_option(option, 4 * tcp_header->doff)) {
    print_tcp_option(option);
  }
}

int tcp_handler(options *opt, char *packet, uint32_t length) {
  struct tcphdr *tcp_header = (struct tcphdr *)packet;

  switch (opt->verbose) {
  case CONCIS:
    printf("TCP, ");
    break;
  case SYNTHETIQUE:
    printf("├─ TCP, Src port: %u, Dst port: %u\n", ntohs(tcp_header->source),
           ntohs(tcp_header->dest));
    break;
  case COMPLET:
    printf("├─ TCP\n");
    printf("│  ├─ Src port: %u\n", ntohs(tcp_header->source));
    printf("│  ├─ Dst port: %u\n", ntohs(tcp_header->dest));
    printf("│  ├─ Sequence number: %u\n", ntohl(tcp_header->seq));
    printf("│  ├─ Acknowledgment number: %u\n", ntohl(tcp_header->ack_seq));
    printf("│  ├─ Data offset: %u bytes\n", tcp_header->doff * 4);
    printf("│  ├─ Flags: 0x%02x\n", tcp_header->th_flags);
    printf("│  │  ├─ URG: %u\n", (tcp_header->th_flags & TH_URG) != 0);
    printf("│  │  ├─ ACK: %u\n", (tcp_header->th_flags & TH_ACK) != 0);
    printf("│  │  ├─ PSH: %u\n", (tcp_header->th_flags & TH_PUSH) != 0);
    printf("│  │  ├─ RST: %u\n", (tcp_header->th_flags & TH_RST) != 0);
    printf("│  │  ├─ SYN: %u\n", (tcp_header->th_flags & TH_SYN) != 0);
    printf("│  │  ├─ FIN: %u\n", (tcp_header->th_flags & TH_FIN) != 0);
    printf("│  ├─ Window size value: %u\n", ntohs(tcp_header->window));
    printf("│  ├─ Checksum: 0x%04x\n", ntohs(tcp_header->check));
    printf("│  ├─ Urgent pointer: %u\n", ntohs(tcp_header->urg_ptr));
    print_tcp_options(tcp_header);
    break;
  }
  return applicatif_handler(opt, packet + 4 * tcp_header->doff,
                            ntohs(tcp_header->source), ntohs(tcp_header->dest),
                            length - 4 * tcp_header->doff);
}