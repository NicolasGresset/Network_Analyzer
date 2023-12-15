#include "../include/capture.h"
#include "../include/erreur.h"
#include <stdlib.h>

#define PROMISCUOUS 1
#define NON_PROMISCUOUS 0
#define PACKET_BUFFER_TIMEOUT 100
#define PACKET_LENGTH 1500
#define INFINI 0
#define UNOPTIMIZED 0

pcap_t *open_capture(options *opt) {
  pcap_t *capture;
  char errbuf[PCAP_ERRBUF_SIZE];
  switch (opt->type) {
  case LIVE:
    if ((capture =
             pcap_open_live(opt->interface, PACKET_LENGTH, NON_PROMISCUOUS,
                            PACKET_BUFFER_TIMEOUT, errbuf)) == NULL) {
      fprintf(stderr, "pcap_open_live : %s\n", errbuf);
      exit(1);
    }
    break;
  case OFFLINE:
    if ((capture = pcap_open_offline(opt->fichier, errbuf)) == NULL) {
      fprintf(stderr, "pcap_open_offline : %s\n", errbuf);
      exit(1);
    }
    break;
  default:
    fprintf(stderr, "Type de capture non reconnu\n");
    exit(1);
  }
  return capture;
}

void start_loop(pcap_t *capture, pcap_handler handler, options *opt) {
  PCHK(pcap_loop(capture, INFINI, handler, (u_char *)opt));
}

void manage_filter(pcap_t *capture, options *opt) {
  if (opt->filtre == NULL) {
    return;
  }

  char errbuf[PCAP_ERRBUF_SIZE];
  bpf_u_int32 netaddr, netmask;
  if (pcap_lookupnet(opt->interface, &netaddr, &netmask, errbuf) ==
      PCAP_ERROR) {
    fprintf(stderr, "error while looking for interface subnet :%s\n", errbuf);
    exit(1);
  }


  PCHK(pcap_compile(capture, &opt->compiled_filter, opt->filtre, UNOPTIMIZED,
                    netmask));
  PCHK(pcap_setfilter(capture, &opt->compiled_filter));
}