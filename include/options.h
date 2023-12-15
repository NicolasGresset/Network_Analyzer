#pragma once
#include <pcap.h>

enum verbose{
    CONCIS = 1,
    SYNTHETIQUE  =2,
    COMPLET = 3
};

enum type{
    LIVE = 1,
    OFFLINE = 2
};


typedef struct options{
    enum verbose verbose;
    enum type type;
     
    char * interface; 
    char * fichier;
    char * filtre;

    struct bpf_program compiled_filter;
} options;

/**
 * @brief Cette fonction gère les options mises en argument
 * 
 * @param argc nombre d'arguments
 * @param argv liste des différentes chaînes de caractères écrites en arguments
 * @param options pointeur vers une structure stockant chaque option mise en argument
 * 
 */
void manage_options(int argc, char ** argv, options *options);


/**
 * @brief Affiche les options : utile pour le debug
 * 
 * @param opt 
 */
void print_options(options *opt);