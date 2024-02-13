#pragma once
#include <pcap.h>

#define PCHK(op)                                                               \
  do {                                                                         \
    if ((op) == PCAP_ERROR)                                                    \
      pcap_raler(#op);                                                              \
  } while (0)

#define CHK(op)                                                                \
  do {                                                                         \
    if ((op) == -1)                                                            \
      raler(#op);                                                              \
  } while (0)

#define NCHK(op)                                                               \
  do {                                                                         \
    if ((op) == NULL)                                                          \
      raler(#op);                                                              \
  } while (0)

/**
 * @brief Display error with pcap_perror then exit
 *
 * @param capture
 * @param msg
 */
void pcap_raler(const char *msg);


/**
 * @brief Display error with perror then exit
 * 
 * @param msg 
 */
void raler(const char *msg);