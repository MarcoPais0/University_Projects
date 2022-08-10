// Biblioteca de funções para processar multibyte characters em C
// v2.1 2021-04-25 João Gabriel Silva

#ifndef LIB_UTF8_H
#define LIB_UTF8_H
#include <stdio.h>
// Todas estas funções pressupõem que a codificação de carateres definida
//   no locale atual é UTF-8

/* Função que retira todos os acentos e cedilhas e passa tudo para
 * minúsculas para permitir comparações e ordenações.
 * Coloca na string dest a string orig transformada.
 * A string dest tem de ter pelo menos o mesmo espaço que a string origem
 * Espera que a string origem esteja codificada em UTF-8 e cria
 * a string destino também em UTF-8 */
void strtobase_u8(char *dest, const char *orig);

/* Função que pasa para maiúsculas todas as minúsculas, mesmo que tenham
 * acentos ou cedilhas. Coloca na string dest a string orig transformada.
 * A string dest tem de ter pelo menos o mesmo espaço que a string origem
 * Espera que a string origem esteja codificada em UTF-8 e cria
 * a string destino também em UTF-8 */
void maiusculas_u8(char *dest, const char *orig);

/* idem, passa a minúsculas */
void minusculas_u8(char *dest, const char *orig);

/* função similar a strlen, com a diferença de que, caso a string contenha carateres multibyte
 * eles contam apenas como um carater */
size_t strlen_u8(const char *s);

/* função similar a fgets, com a diferença de que, caso a linha a ler seja
 * maior do que o espaço disponível, garantidamente não corta a meio um carater
 * multibyte.
 * Guarda o \n na string lida (se chegar ao fim antes do espaço disponível se esgotar)
 * "count" é o número de bytes disponíveis em "s"
 * Devolve s, exceto se não conseguir ler qualquer carater (por atingir logo EOF), caso em
 * que devolve NULL. Se ocorrer qualquer outro erro de leitura também devolve NULL */
char *fgets_u8(char *s, int count, FILE *stream);

/* função similar a fgets_u8, mas recebe a indicação do limite de carateres,
 * não do número de bytes disponíveis para receber a string lida.
 * A string s tem de ter espaço para pelo menos (count * MB_CUR_MAX + 1) bytes */
char *fgets_c_u8(char *s, int count, FILE *stream);
#endif //LIB_UTF8_H
