//Marco da Cruz Pais Nº2019218981
#ifndef PPP_TRABALHO3_PARTE2_STRUCT_H
#define PPP_TRABALHO3_PARTE2_STRUCT_H

#include <stdbool.h>

#define MAX_WORD 100

struct lnode {
    int bytes;
    struct lnode *next;
};

struct no {
    char palavra[MAX_WORD];
    struct lnode *first;
    struct no *left;
    struct no *right;
};

struct binary_tree {
    struct no *raiz;
};

void init_tree(struct binary_tree *pa);

bool tree_insert(struct binary_tree *pa, char *word, int bytes);

void print_tree(struct no *p);

void search_sequence(struct no *p, char *seq);

void search_word(struct no *p, char *seq, char *file);

#endif
