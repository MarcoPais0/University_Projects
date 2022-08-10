/*META 4 - COMPILADORES 2021-2022*/
/*MARCO PAIS Nº 2019218981*/
/*TIAGO OLIVEIRA Nº 2019219068*/

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "struct.h"

typedef struct globalTable{ // lista ligada de funcoes e variaveis globais
    char* name;
    char* type;
    int func;
    int declared;
    struct funcParams* params;
    struct funcVars* vars;
    struct globalTable *next;
} globalTable;

typedef struct funcParams{ // representa uma lista ligada de parametros de uma funcao
    char* name;
    char* type;
    struct funcParams *next;
} funcParams;

typedef struct funcVars{ // representa uma lista ligada de parametros de uma funcao
    int param;
    char* name;
    char* type;
    struct funcVars *next;
} funcVars;

typedef struct erros{
    int linha;
    int column;
    char *erro;
    struct erros *next;
} erros;

void criaTabelas(no_ast* atual);
void printTabelas();
void addFunc(no_ast* atual);
void addFuncParams(no_ast* atual, globalTable* func);
void addGlobalVar(no_ast* atual);
int existsGlobal(char* name, no_ast* atual);
void addFuncLocal(no_ast* atual);
globalTable* getFuncPointer(char* name);
void addFuncLocalVar(no_ast *atual, globalTable *func);
int checkLocalVarUsed(no_ast *atual, char *name);
char* anotaAst(no_ast *atual, globalTable *func);
char* addNotaId(no_ast *atual, globalTable *func);
char* addNotaFunc(no_ast *atual, globalTable *func);
void addErro(char *s, int linha, int column);
void printErros();

#endif //!SEMANTICS_H
