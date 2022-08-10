/*META 4 - COMPILADORES 2021-2022*/
/*MARCO PAIS Nº 2019218981*/
/*TIAGO OLIVEIRA Nº 2019219068*/

#ifndef CODE_H
#define CODE_H

#include "semantics.h"

void geraCode();
void geraFunc(globalTable *func);
void geraFuncHeader(globalTable *func);
void geraFuncParams(globalTable *func);
void geraFuncLocalVars(globalTable *func);
no_ast* getFuncNode(char *name, no_ast * atual);
void geraOperacoes(no_ast* atual, globalTable *func, int ret);
void printStr();

#endif
