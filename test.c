#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

char *ether_ntoa(uint8_t addr[ETH_ALEN]) {
  char *ethernet_address_readable = malloc(ETH_ALEN * 2 + ETH_ALEN);
  for (int i = 0; i < ETH_ALEN; i++) {
    sprintf(ethernet_address_readable + i * 3, "%02hhx", addr[i]);
    if (i != ETH_ALEN - 1) {
      sprintf(ethernet_address_readable + i * 3 + 2, ":");
    }
  }
  ethernet_address_readable[ETH_ALEN * 2 + ETH_ALEN - 1] = '\0';
  return ethernet_address_readable;
}

int main2(void) {
  struct ether_header hdr;
  uint8_t adr[ETH_ALEN] = {0x12, 0x23, 0xfe, 0x66, 0xab, 0xc2};
  char *adr_readable = ether_ntoa(adr);
  printf("chaine : %s\n", adr_readable);

  return 0;
}

int main(void) {
  char *a = "H";
  struct in_addr in_addr = {.s_addr = 40};
  char *b = inet_ntoa(in_addr);
  char *c = malloc(sizeof(*c));
  printf("%p\n%p\n%p\n%ld\n", a, b, c, c - b);
  free(c);
  free(b);
  return 0;
}