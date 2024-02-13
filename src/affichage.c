#include "../include/affichage.h"
#include <stdio.h>
#include <string.h>

void display_with_prefix(const char *chaine, const char *prefixe) {
  char *ligne = strtok((char *)chaine, "\n");
  while (ligne != NULL) {
    printf("%s%s\n", prefixe, ligne);
    ligne = strtok(NULL, "\n");
  }
}

void remove_carriage_return(char *inputString) {
    char *readPtr = inputString;
    char *writePtr = inputString;

    while (*readPtr != '\0') {
        if (*readPtr != '\r') {
            *writePtr = *readPtr;
            writePtr++;
        }
        readPtr++;
    }

    *writePtr = '\0'; // Terminaison de la chaîne après la copie
}

void display_as_hexa(const char *chaine, int length) {
    int i = 0;
    while (i < length) {
        printf("%02x ", chaine[i]);
        i++;
    }
}