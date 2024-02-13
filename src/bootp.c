#include "../include/bootp.h"
#include "../include/affichage.h"
#include "../include/applicatif.h"
#include "../include/erreur.h"
#include <stdlib.h>

#define IPV4_STR_LENGTH 17
#define MAGIC_COOKIE 0x63825363

struct bootp_header {
  uint8_t op;
  uint8_t htype;
  uint8_t hlen;
  uint8_t hops;
  uint32_t xid;
  uint16_t secs;
  uint16_t flags;
  uint32_t ciaddr;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t giaddr;
  uint8_t chaddr[16];
  uint8_t sname[64];
  uint8_t file[128];
  uint8_t options[64];
};

struct bootp_option {
  uint8_t type;
  uint8_t length;
  uint8_t value[];
};

char *get_bootp_sender(uint16_t src_port) {
  if (src_port == BOOTP_REQUEST_PORT) {
    return "client";
  } else if (src_port == BOOTP_REPLY_PORT) {
    return "server";
  } else {
    return "unknown";
  }
}

void display_option_type(struct bootp_option *option) {
  switch (option->type) {
  case 1:
    printf("│  │  ├─ option : Subnet mask\n");
    break;
  case 2:
    printf("│  │  ├─ option : Time offset\n");
    break;
  case 3:
    printf("│  │  ├─ option : Router\n");
    break;
  case 6:
    printf("│  │  ├─ option : Domain name server\n");
    break;
  case 12:
    printf("│  │  ├─ option : Host name\n");
    break;
  case 15:
    printf("│  │  ├─ option : Domain name\n");
    break;
  case 28:
    printf("│  │  ├─ option : Broadcast address\n");
    break;
  case 44:
    printf("│  │  ├─ option : NetBIOS over TCP/IP name server\n");
    break;
  case 53:
    printf("│  │  ├─ option : DHCP message type\n");
    break;
  case 55:
    printf("│  │  ├─ option : Parameter request list\n");
    break;
  default:
    printf("│  │  ├─ option : %d (unknown)\n", option->type);
    break;
  }
}

void display_option_value(struct bootp_option *option) {
  char *payload = get_payload((char *)option->value, option->length);
  switch (option->type) {
  case 53:
    switch (*option->value) {
    case 1:
      printf("│  │  │  ├─ value : DHCPDISCOVER\n");
      break;
    case 2:
      printf("│  │  │  ├─ value : DHCPOFFER\n");
      break;
    case 3:
      printf("│  │  │  ├─ value : DHCPREQUEST\n");
      break;
    case 4:
      printf("│  │  │  ├─ value : DHCPDECLINE\n");
      break;
    case 5:
      printf("│  │  │  ├─ value : DHCPACK\n");
      break;
    case 7:
      printf("│  │  │  ├─ value : DHCPRELEASE\n");
      break;
    }
    break;
  default:
    printf("│  │  │  ├─ value :\n");
    display_with_prefix(payload, PREFIX);
    break;
  }
  free(payload);
}

void display_bootp_options(char *options) {
  struct bootp_option *option = (struct bootp_option *)options;

  while (option->type != 0xff && (char *)option < options + 64) {
    display_option_type(option);
    printf("│  │  ├─ length : %d\n", option->length);
    display_option_value(option);
    option = (struct bootp_option *)((char *)option + option->length + 2);
  }
}

int bootp_handler(options *opt, const char *packet, uint16_t src_port,
                  uint16_t dst_port, uint32_t length) {
  (void)dst_port;
  (void)length;
  struct bootp_header *bootp = (struct bootp_header *)packet;

  char ciaddr[IPV4_STR_LENGTH];
  char yiaddr[IPV4_STR_LENGTH];
  char siaddr[IPV4_STR_LENGTH];
  char giaddr[IPV4_STR_LENGTH];
  NCHK(inet_ntop(AF_INET, &bootp->ciaddr, ciaddr, IPV4_STR_LENGTH));
  NCHK(inet_ntop(AF_INET, &bootp->yiaddr, yiaddr, IPV4_STR_LENGTH));
  NCHK(inet_ntop(AF_INET, &bootp->siaddr, siaddr, IPV4_STR_LENGTH));
  NCHK(inet_ntop(AF_INET, &bootp->giaddr, giaddr, IPV4_STR_LENGTH));

  char *chaddr = get_payload((char *)bootp->chaddr, 16);
  char *sname = get_payload((char *)bootp->sname, 64);
  char *file = get_payload((char *)bootp->file, 128);

  switch (opt->verbose) {
  case CONCIS:
    printf("BOOTP\n");
    break;
  case SYNTHETIQUE:
    printf("├─ BOOTP, from %s\n", get_bootp_sender(src_port));
    break;
  case COMPLET:
    printf("├─ BOOTP\n");
    printf("│  ├─ Payload : \n");
    printf("│  │  ├─ op : %d\n", bootp->op);
    printf("│  │  ├─ htype : %d\n", bootp->htype);
    printf("│  │  ├─ hlen : %d\n", bootp->hlen);
    printf("│  │  ├─ hops : %d\n", bootp->hops);
    printf("│  │  ├─ xid : %d\n", bootp->xid);
    printf("│  │  ├─ secs : %d\n", bootp->secs);
    printf("│  │  ├─ flags : %d\n", bootp->flags);
    printf("│  │  ├─ ciaddr : %s\n", ciaddr);
    printf("│  │  ├─ yiaddr : %s\n", yiaddr);
    printf("│  │  ├─ siaddr : %s\n", siaddr);
    printf("│  │  ├─ giaddr : %s\n", giaddr);
    printf("│  │  ├─ chaddr :\n");
    display_with_prefix(chaddr, PREFIX);
    printf("│  │  ├─ sname :\n");
    display_with_prefix(sname, PREFIX);
    printf("│  │  ├─ file :\n");
    display_with_prefix(file, PREFIX);

    if (ntohl(*(uint32_t *)bootp->options) == MAGIC_COOKIE) {
      display_bootp_options((char *)bootp->options + 4);
      break;
    }
  }
  free(chaddr);
  free(sname);
  free(file);
  return 0;
}