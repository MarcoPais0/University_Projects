// Biblioteca de funções para processar multibyte characters em C
// v2 2021-03-19 João Gabriel Silva

#include <stdio.h>
#include <string.h>
#include "lib-utf8.h"

static char acentuadas[][8] =
        {"á", "Á", "à", "À", "ã", "Ã", "â", "Â", "ä", "Ä", "ç", "Ç",
         "é", "É", "è", "È", "ê", "Ê", "í", "Í", "ì", "Ì", "î", "Î",
         "ó", "Ó", "ò", "Ò", "õ", "Õ", "ô", "Ô", "ö", "Ö", "ú", "Ú",
         "ù", "Ù", "û", "Û", "\0"};
static char base[] =
        {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c', 'c',
         'e', 'e', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 'i', 'i',
         'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'u', 'u',
         'u', 'u', 'u', 'u', '\0'};
static char minusculas[][8] =
        {"á", "à", "ã", "â", "ä", "ç", "é", "è", "ê", "í", "ì", "î",
         "ó", "ò", "õ", "ô", "ö", "ú", "ù", "û", "\0"};
static char maiusculas[][8] =
        {"Á", "À", "Ã", "Â", "Ä", "Ç", "É", "È", "Ê", "Í", "Ì", "Î",
         "Ó", "Ò", "Õ", "Ô", "Ö", "Ú", "Ù", "Û", "\0"};

static int comp_mchar(const char *um, const char *outro);

void strtobase_u8(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    // percorrer a string origem
    c = (unsigned char) orig[0];
    while (c != 0) {
        if ((c & 0x80) == 0) {
            // é um carater ASCII, basta converter as letras para minusculas
            if (c <= 'Z' && c >= 'A')
                c = c + 'a' - 'A';
            i++;
            dest[j] = (char) c;
            j++;
        } else {
            // é um carater UTF-8 com mais de um byte
            int k = 0, tc = 0;
            while (acentuadas[k][0] != 0) {
                // se for uma das letras acentuadas, retira-se o acento
                if ((tc = comp_mchar(&acentuadas[k][0], &orig[i])) != 0) {
                    // encontramos uma minúscula acentuada
                    dest[j] = base[k];
                    j++;
                    i = i + tc;
                    break;
                }
                k++;
            }
            if (tc == 0) {
                // não é uma letra acentuada, logo é de copiar para
                //   o destino, sem alterações, todos os carateres UTF-8
                do {
                    dest[j] = orig[i];
                    i++;
                    j++;
                } while ((orig[i] & 0xC0) == 0x80);
            }
        }
        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}

void maiusculas_u8(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    // percorrer a string origem
    c = (unsigned char) orig[0];
    while (c != 0) {
        if ((c & 0x80) == 0) {
            // é um carater ASCII, basta converter as letras para maiusculas
            if (c <= 'z' && c >= 'a')
                c = c + ('A' - 'a');
            i++;
            dest[j] = (char) c;
            j++;
        } else {
            // é um carater UTF-8 com mais de um byte
            int k = 0, tc = 0;
            while (minusculas[k][0] != 0) {
                // se for uma das letras acentuadas, retira-se o acento
                if ((tc = comp_mchar(&minusculas[k][0], &orig[i])) != 0) {
                    // encontramos uma minúscula acentuada
                    strcpy(&dest[j], &maiusculas[k][0]);
                    j = j + tc;
                    i = i + tc;
                    break;
                }
                k++;
            }
            if (tc == 0) {
                // não é uma letra acentuada, logo é de copiar para
                //   o destino, sem alterações, todos os carateres UTF-8
                do {
                    dest[j] = orig[i];
                    i++;
                    j++;
                } while ((orig[i] & 0xC0) == 0x80);
            }
        }
        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}

void minusculas_u8(char *dest, const char *orig) {
    int i = 0, j = 0, c;
    // percorrer a string origem
    c = (unsigned char) orig[0];
    while (c != 0) {
        if ((c & 0x80) == 0) {
            // é um carater ASCII, basta converter as letras para minusculas
            if (c <= 'Z' && c >= 'A')
                c = c + ('a' - 'A');
            i++;
            dest[j] = (char) c;
            j++;
        } else {
            // é um carater UTF-8 com mais de um byte
            int k = 0, tc = 0;
            while (maiusculas[k][0] != 0) {
                // se for uma das letras acentuadas, retira-se o acento
                if ((tc = comp_mchar(&maiusculas[k][0], &orig[i])) != 0) {
                    // encontramos uma minúscula acentuada
                    strcpy(&dest[j], &minusculas[k][0]);
                    j = j + tc;
                    i = i + tc;
                    break;
                }
                k++;
            }
            if (tc == 0) {
                // não é uma letra acentuada, logo é de copiar para
                //   o destino, sem alterações, todos os carateres UTF-8
                do {
                    dest[j] = orig[i];
                    i++;
                    j++;
                } while ((orig[i] & 0xC0) == 0x80);
            }
        }
        c = (unsigned char) orig[i];
    }
    dest[j] = 0;
}

size_t strlen_u8(const char *s) {
    size_t tam = 0, i = 0;
    while (s[i] != 0) {
        if ((s[i] & 0xC0) != 0x80)
            tam++;
        i++;
    }
    return tam;
}

char *fgets_u8(char *s, int count, FILE *stream) {
    int i, c, restante;
    restante = count-1;
    for (i = 0; restante > 0 && (c = getc(stream)) != EOF && c != '\n'; ++i, restante--) {
        s[i] = (char) c;
        if (restante < 4) {
            if ((c & 0xF8) == 0xF0) {
                // este é o carater inicial de uma sequência de 4 bytes - não cabe
                ungetc(c, stream);
                break;
            }
            if (restante < 3) {
                if ((c & 0xF0) == 0xE0) {
                    // este é o carater inicial de uma sequência de 3 bytes - não cabe
                    ungetc(c, stream);
                    break;
                }
            }
            if (restante < 2) {
                if ((c & 0xE0) == 0xC0) {
                    // este é o carater inicial de uma sequência de 2 bytes - não cabe
                    ungetc(c, stream);
                    break;
                }
            }
        }
    }
    if (c == '\n') {
        s[i] = (char) c;
        ++i;
    }
    if (c == EOF && (restante == count-1))
        return NULL;
    else {
        s[i] = '\0';
        return s;
    }
}

char *fgets_c_u8(char *s, int count, FILE *stream) {
    int i, c, restante;
    i = 0;
    for (restante = count; restante > 0 && (c = getc(stream)) != EOF && c != '\n'; restante--) {
        s[i++] = (char) c;
        if ((c & 0xF8) == 0xF0) {
            // este é o carater inicial de uma sequência de 4 bytes
            s[i++] = (char) getc(stream);
            s[i++] = (char) getc(stream);
            s[i++] = (char) getc(stream);
        } else if ((c & 0xF0) == 0xE0) {
            // este é o carater inicial de uma sequência de 3 bytes
            s[i++] = (char) getc(stream);
            s[i++] = (char) getc(stream);
        } else if ((c & 0xE0) == 0xC0) {
            // este é o carater inicial de uma sequência de 2 bytes
            s[i++] = (char) getc(stream);
        }
    }
    if (c == '\n') {
        s[i] = (char) c;
        ++i;
    }
    if (c == EOF && (restante == count))
        return NULL;
    else {
        s[i] = '\0';
        return s;
    }
}

// comparar dois carateres multibyte, sob a forma de string, tendo o primeiro de ser terminado por zero
// devolve zero se são diferentes; devolve o número de bytes do carater se são iguais
static int comp_mchar(const char *um, const char *outro) {
    int i = 0;
    while (um[i] != 0) {
        if (um[i] != outro[i])
            return 0;
        i++;
    }
    return i;
}

