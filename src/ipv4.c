#include "../include/ipv4.h"
#include "../include/erreur.h"
#include "../include/niveau3.h"
#include "../include/niveau4.h"
#include <netinet/ip.h>
#define IPV4_STR_LENGTH 17

struct ipv4_kind {
  uint8_t flag : 1;
  uint8_t class : 2;
  uint8_t number : 5;
};

union ipv4_option {
  struct ipv4_kind kind;
  struct {
    struct ipv4_kind kind;
    uint8_t length;
    char data[];
  } tlv;
};

void print_ipv4_option_kind(struct ipv4_kind kind) {
  printf("│  │  ├─ Kind: %u\n", kind.class);
  printf("│  │  │  ├─ Flag: %u\n", kind.flag);
  printf("│  │  │  ├─ Class: %u\n", kind.class);
  printf("│  │  │  ├─ Number: %u\n", kind.number);
}

void print_ipv4_option(union ipv4_option *option) {
  switch (option->kind.class) {
  case IPOPT_CONTROL:
    switch (option->kind.number) {
    case IPOPT_EOL:
      printf("│  ├─ Option : End of Option List\n");
      break;
    case IPOPT_NOP:
      printf("│  ├─ Option : No Operation\n");
      break;
    case IPOPT_RR:
      printf("│  ├─ Option : Record route\n");
      printf("│  │  ├─ Length : %hhu\n", option->tlv.length);
      printf("│  │  ├─ Pointer : %hhu\n", option->tlv.data[0]);
      printf("│  │  ├─ IP adresses : ");
      for (int i = 1; i < option->tlv.length; i += 4) {
        char adress[IPV4_STR_LENGTH];
        NCHK(inet_ntop(AF_INET, &option->tlv.data[i], adress, IPV4_STR_LENGTH));
        printf("%s ", adress);
      }
      break;
    default:
      printf("│  ├─ Option : Unknown, the remaining options are ignored\n");
      break;
    }
    break;
  default:
    printf("│  ├─ Option : Unknown, the remaining options are ignored\n");
    break;
  }
  print_ipv4_option_kind(option->kind);
}

union ipv4_option *next_ipv4_option(union ipv4_option *option,
                                    uint8_t header_length) {
  switch (option->kind.class) {
  case IPOPT_CONTROL:
    switch (option->kind.number) {
    case IPOPT_EOL:
      return option;
    case IPOPT_NOP:
      return (union ipv4_option *)((char *)option + 1);
    case IPOPT_RR:
      return (union ipv4_option *)((char *)option + option->tlv.length);
    default:
      // si on ne connait pas l'option, on ignore les prochaines pour
      // pouvoir continuer le parsage
      return (union ipv4_option *)((char *)option + header_length);
    }
  default:
    // idem
    return (union ipv4_option *)((char *)option + header_length);
  }
}

int is_end_ipv4_options(union ipv4_option *option,
                        const struct iphdr *ip_header) {
  return (((char *)option >= (char *)ip_header + 4 * ip_header->ihl) ||
          (option->kind.class == 0 && option->kind.number == IPOPT_EOL));
}

/**
 * @brief Affiche les options IPv4 contenues dans l'en-tête ip_header'
 *
 * @param packet
 */
void print_ipv4_options(const struct iphdr *ip_header) {
  char *start_options = (char *)ip_header + sizeof(struct iphdr);

  union ipv4_option *option;
  for (option = (union ipv4_option *)start_options;
       !is_end_ipv4_options(option, ip_header);
       option = next_ipv4_option(option, ip_header->ihl * 4)) {
    print_ipv4_option(option);
  }
}

int ipv4_handler(options *opt, const char *packet, uint32_t length) {
  const struct iphdr *ip_header = (const struct iphdr *)packet;

  char adress_src[IPV4_STR_LENGTH];
  char adress_dst[IPV4_STR_LENGTH];
  NCHK(inet_ntop(AF_INET, &ip_header->saddr, adress_src, IPV4_STR_LENGTH));
  NCHK(inet_ntop(AF_INET, &ip_header->daddr, adress_dst, IPV4_STR_LENGTH));

  switch (opt->verbose) {
  case CONCIS:
    printf("IPv4, ");
    break;
  case SYNTHETIQUE:
    printf("├─ IPv4, Src: %s, Dst: %s\n", adress_src, adress_dst);
    break;
  case COMPLET:
    printf("├─ IPv4\n");
    printf("│  ├─ Version: %u, Header length: %u bytes, TOS: 0x%02x\n",
           ip_header->version, ip_header->ihl * 4, ip_header->tos);
    printf("│  ├─ Total length: %u bytes, Identification: %u\n",
           ntohs(ip_header->tot_len), ntohs(ip_header->id));
    printf("│  ├─ Flags: 0x%02x, Fragment offset: %u\n",
           ip_header->frag_off & 0xE000, ip_header->frag_off & 0x1FFF);
    printf("│  ├─ TTL: %u, Protocol: %u, Checksum: 0x%04x\n", ip_header->ttl,
           ip_header->protocol, ntohs(ip_header->check));
    printf("│  ├─ Source: %s, Destination: %s\n", adress_src, adress_dst);
    print_ipv4_options(ip_header);
    break;
  }

  char *level4_packet = (char *)packet + ip_header->ihl * 4;

  enum niveau4_type type = type_niveau4(ip_header->protocol);
  return niveau4_handler(opt, level4_packet, type, length - ip_header->ihl * 4);
}