//Marco da Cruz Pais Nº2019218981
#include "struct.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "lib-utf8.h"

// inicializa uma árvore
void init_tree(struct binary_tree *pa) {
    pa->raiz = NULL;
}

struct no *addtree(struct no *pr, struct no *p);

// inicializa o novo nó da árvore e adiciona-o à mesma
bool tree_insert(struct binary_tree *pa, char *word, int bytes) {
    struct no *p;
    struct lnode *aux;
    p = (struct no *) malloc(sizeof(struct no));
    aux = (struct lnode *) malloc(sizeof(struct lnode));
    if (p == NULL || aux == NULL)
        return false;
    aux->bytes = bytes;
    aux->next = NULL;
    strcpy(p->palavra, word);
    p->left = p->right = NULL;
    p->first = aux;
    pa->raiz = addtree(pa->raiz, p);
    return true;
}

// adiciona o novo nó à árvore procurando por uma folha vazia ou por uma palavra igual, sendo que neste caso
// apenas adiciona a nova posição à lista ligada
struct no *addtree(struct no *pr, struct no *p) {
    int cond;
    char nome1[MAX_WORD + 1], nome2[MAX_WORD + 1];
    if (pr == NULL) {
        pr = p;
        return pr;
    }
    strtobase_u8(nome1, p->palavra);
    strtobase_u8(nome2, pr->palavra);
    cond = strcmp(nome1, nome2);
    if (cond == 0) {
        struct lnode *aux = pr->first;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = (struct lnode *) malloc(sizeof(struct lnode));
        aux->next->bytes = p->first->bytes;
        aux->next->next = NULL;
        free(p);
        return pr;
    }
    if (cond > 0)
        pr->right = addtree(pr->right, p);
    if (cond < 0)
        pr->left = addtree(pr->left, p);
    return pr;
}

// dá print do conteúdo da árvore
void print_tree(struct no *p) {
    if (p != NULL) {
        print_tree(p->left);
        struct lnode *aux = p->first;
        printf("%s", p->palavra);
        while (aux != NULL) {
            printf(" %d", aux->bytes);
            aux = aux->next;
        }
        printf("\n");
        print_tree(p->right);
    }
}

// procura uma sequência de caracteres na árvore e apresenta todas as palavras que comecem por ela e as suas ocorrências
void search_sequence(struct no *p, char *seq) {
    if (p != NULL) {
        search_sequence(p->left, seq);
        int cond;
        char nome1[MAX_WORD], nome2[MAX_WORD];
        strtobase_u8(nome1, p->palavra);
        strtobase_u8(nome2, seq);
        nome1[strlen(seq)] = 0;
        cond = strcmp(nome1, nome2);
        if (cond == 0) {
            struct lnode *aux = p->first;
            while (aux != NULL) {
                printf("%s %d\n", p->palavra, aux->bytes);
                aux = aux->next;
            }
        }
        search_sequence(p->right, seq);
    }
}

void show_context(int bytes, char *file);

// procura por uma palavra na árvore e depois mostra o seu contexto nas várias posições por ordem decrescente
void search_word(struct no *p, char *seq, char *file) {
    if (p != NULL) {
        search_word(p->left, seq, file);
        int cond;
        char nome1[MAX_WORD], nome2[MAX_WORD];
        strtobase_u8(nome1, p->palavra);
        strtobase_u8(nome2, seq);
        cond = strcmp(nome1, nome2);
        if (cond == 0) {
            int bytes[100], i = -1;
            struct lnode *aux = p->first;
            while (aux != NULL) {
                i++;
                bytes[i] = aux->bytes;
                aux = aux->next;
            }
            for (; i >= 0; i--) {
                printf("Contexto: %s - %d\n", seq, bytes[i]);
                show_context(bytes[i], file);
            }
        }
        search_word(p->right, seq, file);
    }
}

// mostra o contexto da palavra na posição "bytes" do ficheiro
void show_context(int bytes, char *file) {
    int c;
    FILE *f = fopen(file, "r");
    fseek(f, bytes, SEEK_SET);
    sleep(1);
    fseek(f, -1, SEEK_CUR);
    sleep(1);
    while (1) {
        if (ftell(f) == 0) {
            break;
        }
        if (fgetc(f) == '.') {
            break;
        }
        fseek(f, -2, SEEK_CUR);
    }
    int count = 0, i = 0;
    char contexto[1000];
    while (count < 2) {
        c = fgetc(f);
        contexto[i] = (char) c;
        // detetar e eliminar o UTF BOM
        if (i == 2 && contexto[0] == (char) 0xEF && contexto[1] == (char) 0xBB && contexto[2] == (char) 0xBF)
            i = 0;
        else
            i++;
        if (c == '.') {
            count++;
        }
    }
    contexto[i] = 0;
    printf("%s\n", contexto);
    if (fclose(f) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return;
    }
}
