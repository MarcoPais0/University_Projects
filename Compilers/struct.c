/*META 4 - COMPILADORES 2021-2022*/
/*MARCO PAIS Nº 2019218981*/
/*TIAGO OLIVEIRA Nº 2019219068*/

#include "struct.h"

no_ast *novoNo(char *tipo, char *valor, int line, int column)
{
    no_ast *no = (no_ast *) malloc(sizeof(no_ast));
    no->tipo = (char *) strdup(tipo);
    no->filho = no->irmao = NULL;
    no->nota = NULL;
    no->line = line;
    no->column = column;

    //Caso não tenha um valor associado (p.ex.: ID(<valor>))
    if (valor == NULL)
    {
        no->valor = NULL;
        return no;
    }
    no->valor = (char *) strdup(valor);
    return no;
}

// Adiciona um "irmão"
void addirmao(no_ast *no, no_ast *novoirmao)
{
    if (no == NULL || novoirmao == NULL)
        return;
    
    no_ast *no_aux = no;

    while (no_aux->irmao)
        no_aux = no_aux->irmao;
    
    no_aux->irmao = novoirmao;
}

void addfilho(no_ast *no, no_ast *filho)
{
    if (no == NULL || filho == NULL)
        return;

    no->filho = filho;
}

void printAST(no_ast *no, int nivel)
{
    if (no == NULL) return;
    
    if (no->tipo != NULL)
    {
        if(strcmp(no->tipo,"NULL")!=0){
            for (int i = 0; i < nivel; i++)
                printf("..");

            if (no->valor != NULL)
                printf("%s(%s)", no->tipo, no->valor);
            else
                printf("%s", no->tipo);

            if (no->nota != NULL){
                if(strcmp(no->nota,"none") != 0)    
                    printf(" - %s", no->nota);
            }

            printf("\n");
            
            if (no->filho != NULL)
                printAST(no->filho, nivel + 1);
            if (no->irmao != NULL)
                printAST(no->irmao, nivel);
        }
        else{
            if (no->filho != NULL)
                printAST(no->filho, nivel);
            if (no->irmao != NULL)
                printAST(no->irmao, nivel);
        }   
    }
}

void freeAST(no_ast *no)
{
    if (no == NULL) return;
    
    freeAST(no->filho);
    freeAST(no->irmao);

    free(no->tipo);
    free(no->valor);
    free(no);
}

void addNota(no_ast *no, char* nota){
    no->nota = (char *) strdup(nota);
}
