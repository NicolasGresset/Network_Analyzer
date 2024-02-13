#include "../include/handler.h"
#include "../include/niveau2.h"

void handler(u_char *args, const struct pcap_pkthdr *header,
             const u_char *packet) {
  options *opt = (options *)args;
  niveau2_handler(opt, (char *)packet, header->len);
}