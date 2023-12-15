#include "../include/options.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Affiche la syntaxe d'utilisation du programme sur le flux stream
 *
 * @param stream
 */
void display_help(FILE *stream) {
  fprintf(stream,
          "Usage: sniffer (-i interface_name | -o file_name) -v <1:3> [-f "
          "filtre] \n");
}

void manage_options(int argc, char **argv, options *options) {
  int opt;
  int verbose_level;
  int interface_set = 0;
  int verbose_set = 0;
  options->filtre = NULL;

  while ((opt = getopt(argc, argv, "i:o:f:v:h")) != -1) {
    switch (opt) {
    case 'i':
      if (interface_set) {
        fprintf(stderr, "Vous ne pouvez pas choisir une interface si vous "
                        "fournissez un fichier offline\n");
        exit(1);
      }
      options->interface = optarg;
      options->type = LIVE;
      interface_set = 1;
      break;
    case 'o':
      if (interface_set) {
        fprintf(stderr, "Vous ne pouvez pas fournir in fichier offline si vous "
                        "choisissez une méthode de capture live\n");
        exit(1);
      }
      options->fichier = optarg;
      options->type = OFFLINE;
      interface_set = 1;
      break;
    case 'f':
      options->filtre = optarg;
      break;
    case 'v':
      verbose_level = atoi(optarg);
      if ((verbose_level < 1) || (verbose_level > 3)) {
        fprintf(
            stderr,
            "Le niveau de verbosité doit être compris entre 1 et 3 inclus\n");
        exit(1);
      }
      options->verbose = verbose_level;
      verbose_set = 1;
      break;
    case 'h':
      display_help(stdout);
      exit(0);
    default:
      display_help(stderr);
      exit(EXIT_FAILURE);
    }
  }
  if (!interface_set) {
    fprintf(stderr, "Vous devez choisir une interface\n");
    exit(1);
  }
  if (!verbose_set) {
    fprintf(stdout, "Pas de niveau de verbosité choisi, mode synthétique "
                    "choisi par défaut\n");
    options->verbose = SYNTHETIQUE;
  }
  if (options->type == OFFLINE && options->filtre != NULL) {
    fprintf(stdout, "Vous ne pouvez pas spécifier de filtre pour une capture "
                    "offline : filtre ignoré\n");
    options->filtre = NULL;
  }
  return;
}

void print_options(options *opt) {
  printf("\n------------\n");
  printf("Niveau de verbosité : ");
  switch (opt->verbose) {
  case CONCIS:
    printf("concis\n");
    break;
  case SYNTHETIQUE:
    printf("synthétique\n");
    break;
  case COMPLET:
    printf("complet\n");
    break;
  default:
    printf("\033[1;31m inconnu \033[0m \n");
    break;
  }

  printf("Type de capture : ");
  switch (opt->type) {
  case LIVE:
    printf("live\n");
    printf("Interface : %s\n", opt->interface);
    break;
  case OFFLINE:
    printf("offline\n");
    printf("Fichier analysé : %s\n", opt->fichier);
    break;
  default:
    printf("\033[1;31m inconnu \033[0m \n");
    break;
  }

  printf("Filtre : ");
  if (opt->filtre == NULL) {
    printf("aucun\n");
  } else {
    printf("%s\n", opt->filtre);
  }
  printf("------------\n");
  return;
}