#include "../include/handler.h"
#include "../include/niveau2.h"

void handler(u_char *args, const struct pcap_pkthdr *header,
             const u_char *packet) {
  options *opt = (options *)args;
  (void)header;

  if (niveau2_handler(opt, (char *)packet) == PROTOCOLE_INCONNU) {
    fprintf(stderr, "protocole inconnu\n");
    return;
  }
}