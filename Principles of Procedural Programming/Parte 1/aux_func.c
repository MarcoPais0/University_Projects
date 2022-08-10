//Marco da Cruz Pais Nº2019218981
#include "aux_func.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//pede o nome do ficheiro ao utilizador
void get_filename(char *filename) {
    printf("Introduza o nome do ficheiro a ler: ");
    scanf("%100s", filename);
}

size_t strlen_u8(const char *s);

//lê o texto caracter a caracter e guarda a localização de cada palavra num ficheiro binário juntamente com a palavra
bool analyze_text(char *filename) {
    FILE *fich1;
    FILE *fich2;
    char word[MAX_WORD];
    char new_filename[8 + MAX_WORD] = "tab_";
    int i, c, j;
    fich1 = fopen(filename, "r");
    if (fich1 == NULL) {
        printf("Ficheiro %s não existe!", filename);
        return false;
    }
    strcat(new_filename, strtok(filename, "."));
    strcat(new_filename, ".bin");
    fich2 = fopen(new_filename, "wb");
    i = 0;
    j = 1;
    while ((c = fgetc(fich1)) != EOF) {
        if (!(ispunct(c) || isspace(c)) || c == '-') {
            word[i] = (char) c;
            i++;
        } else {
            if (strlen_u8(word) > 3) {
                word[i] = 0;
                int aux = j - i - 1;
                fwrite(&aux, sizeof(int), 1, fich2);
                fwrite(&word, MAX_WORD, 1, fich2);
            }
            memset(word, 0, MAX_WORD);
            i = 0;
        }
        j++;
    }
    if (fclose(fich1) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return 0;
    }
    if (fclose(fich2) == EOF) {
        fprintf(stderr, "Erro ao fechar o ficheiro de escrita\n");
        return 0;
    }
    printf("Texto lido com sucesso!");
    return true;
}

/* função similar a strlen, com a diferença de que, caso a string contenha carateres multibyte
 * eles contam apenas como um carater */
//utilizo esta função para comparar o tamanho real das palavras com o valor mínimo necessário
size_t strlen_u8(const char *s) {
    size_t tam = 0, i = 0;
    while (s[i] != 0) {
        if ((s[i] & 0xC0) != 0x80)
            tam++;
        i++;
    }
    return tam;
}