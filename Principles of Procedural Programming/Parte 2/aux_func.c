//Marco da Cruz Pais Nº2019218981
#include "aux_func.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// pede o nome do ficheiro ao utilizador
void get_filename(char *filename) {
    printf("Introduza o nome do ficheiro a ler: ");
    fgets(filename, MAX_WORD, stdin);
    filename[strlen(filename) - 1] = 0;
}

// lê as palavras e as posições do ficheiro binário e adiciona-as à árvore
void insert_words(char *bin, struct binary_tree *pa) {
    FILE *fich;
    fich = fopen(bin, "rb");
    char word[MAX_WORD];
    int bytes;
    while (fread(&bytes, sizeof(int), 1, fich) && fread(word, MAX_WORD, 1, fich)) {
        tree_insert(pa, word, bytes);
    }
    if (fclose(fich) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return;
    }
}

// apresenta o menu ao utilizador e lê e processa a opçâo inserida
bool menu(struct binary_tree *pa, char *file) {
    printf("\n\n-------------- MENU --------------\n"
           "1 - Mostrar uma palavra em contexto.\n"
           "2 - Mostrar todas as palavras começadas por uma letra ou sequência de letras.\n"
           "3 - Sair.\n"
           "Opção: ");
    char escolha[10], *p;
    fgets(escolha, 10, stdin);
    int n = (int) strtol(escolha, &p, 10);
    if (escolha == p || (n != 1 && n != 2 && n != 3)) {
        printf("\nOpção inválida!\n");
    }

    // mostra a palavra em contexto, ou seja, a frase em que se encontra e a posterior
    if (n == 1) {
        char word[MAX_WORD];
        printf("\nIntroduza a palavra: ");
        fgets(word, MAX_WORD, stdin);
        word[strlen(word) - 1] = 0;
        if (strlen(word) > 0) {
            search_word(pa->raiz, word, file);
            printf("Fim da lista!\n");
        } else {
            printf("\nPalavra inválida!");
        }
    }
    // mostrar todas as palavras que comecem por uma sequência de letras
    if (n == 2) {
        char seq[MAX_WORD];
        printf("\nIntroduza a sequência: ");
        fgets(seq, MAX_WORD, stdin);
        seq[strlen(seq) - 1] = 0;
        if (strlen(seq) > 0) {
            search_sequence(pa->raiz, seq);
            printf("Fim da lista!\n");
        } else {
            printf("\nSequência inválida!\n");
        }
    }
    // sair
    if (n == 3) return false;
    return true;
}
