//Marco da Cruz Pais Nº2019218981
#include "aux_func.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char file1[MAX_WORD];
    if (argc < 2) {
        get_filename(file1);
    } else {
        strcpy(file1, argv[1]);
    }
    char aux[MAX_WORD];
    strcpy(aux, file1);
    char file2[8 + MAX_WORD] = "tab_";
    strcat(file2, strtok(aux, "."));
    strcat(file2, ".bin");

    // verifica existência de ficheiros
    FILE *f = fopen(file1, "r");
    if (f == NULL) {
        printf("Ficheiro de texto não existe!");
        return 0;
    } else {
        if (fclose(f) == EOF) {
            fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
            return 0;
        }
    }
    f = fopen(file2, "rb");
    if (f == NULL) {
        printf("Ficheiro binário não existe!");
        return 0;
    } else {
        if (fclose(f) == EOF) {
            fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
            return 0;
        }
    }

    struct binary_tree tree;
    init_tree(&tree);

    insert_words(file2, &tree);

    print_tree(tree.raiz);

    while (menu(&tree, file1));
    return 0;
}
