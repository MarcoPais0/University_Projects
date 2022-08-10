/*META 4 - COMPILADORES 2021-2022*/
/*MARCO PAIS Nº 2019218981*/
/*TIAGO OLIVEIRA Nº 2019219068*/

#include "semantics.h"
#include <ctype.h>

globalTable *Head = NULL;
erros *Err = NULL;

int semanticError = 0;

void criaTabelas(no_ast *atual)
{
    atual = atual->filho;       // salta "Program"
    no_ast *globalHead = atual; // guarda o inicio da lista de funções e variaveis globais

    // adiciona funcoes e variaveis globais
    while (strcmp(atual->tipo, "FuncDecl") == 0 || strcmp(atual->tipo, "VarDecl") == 0)
    {
        if (strcmp(atual->tipo, "FuncDecl") == 0)
        {
            addFunc(atual);
        }
        else if (strcmp(atual->tipo, "VarDecl") == 0)
        {
            addGlobalVar(atual);
        }

        atual = atual->irmao; // passa para a proxima declaracao
        if (atual == NULL)
        {
            break;
        }
    }

    atual = globalHead;

    // adiciona vars locais e anota a AST
    while (strcmp(atual->tipo, "FuncDecl") == 0 || strcmp(atual->tipo, "VarDecl") == 0)
    {
        // se encontrar uma função
        if (strcmp(atual->tipo, "FuncDecl") == 0)
            addFuncLocal(atual);

        atual = atual->irmao; // passa para a proxima declaracao
        if (atual == NULL)
        {
            break;
        }
    }

    printErros();
}

void printTabelas()
{
    printf("===== Global Symbol Table =====\n");
    globalTable *aux = Head;
    while (aux)
    {
        printf("%s", aux->name);
        if (aux->func)
        {
            printf("\t(");
            funcParams *paramsAux = aux->params;
            if (paramsAux)
            {
                printf("%s", paramsAux->type);
                paramsAux = paramsAux->next;
                while (paramsAux)
                {
                    printf(",%s", paramsAux->type);
                    paramsAux = paramsAux->next;
                }
            }

            printf(")");
        }
        else
        {
            printf("\t");
        }
        printf("\t%s\n", aux->type);
        aux = aux->next;
    }

    aux = Head;
    while (aux)
    {
        if (aux->func)
        {
            printf("\n===== Function ");
            printf("%s(", aux->name);
            funcParams *paramsAux = aux->params;
            if (paramsAux)
            {
                printf("%s", paramsAux->type);
                paramsAux = paramsAux->next;
                while (paramsAux)
                {
                    printf(",%s", paramsAux->type);
                    paramsAux = paramsAux->next;
                }
            }
            printf(") Symbol Table =====\n");
            printf("%s\t\t%s\n", "return", aux->type); // print do return

            paramsAux = aux->params;
            while (paramsAux)
            {
                printf("%s\t\t%s\tparam\n", paramsAux->name, paramsAux->type); // print dos parametros como variaveis locais
                paramsAux = paramsAux->next;
            }

            funcVars *varsAux = aux->vars; // print das variaveis locais
            while (varsAux)
            {
                printf("%s\t\t%s\n", varsAux->name, varsAux->type);
                varsAux = varsAux->next;
            }
        }

        aux = aux->next;
    }
    printf("\n");
}

void addFunc(no_ast *atual)
{
    globalTable *aux = Head;

    char *funcName = (char *)malloc(sizeof(char) * strlen(atual->filho->filho->valor) + 1);
    strcpy(funcName, atual->filho->filho->valor); // FuncDecl->FuncHeader->Id(nome)

    char *funcType = (char *)malloc(sizeof(char) * strlen(atual->filho->filho->irmao->tipo) + 1);
    strcpy(funcType, atual->filho->filho->irmao->tipo); // FuncDecl->FuncHeader->Id->FuncParams
    funcType[0] = tolower(funcType[0]);

    // verifica se já existe uma função com o mesmo nome
    if (existsGlobal(funcName, atual->filho->filho))
    {
        return;
    }

    if (!Head)
    {
        // primeiro elemento da lista global
        Head = (globalTable *)malloc(sizeof(globalTable));

        if (strcmp(funcType, "FuncParams") == 0 || strcmp(funcType, "funcParams") == 0)
        {
            funcType = "none";
        }

        Head->func = 1;
        Head->declared = 0;
        Head->name = funcName;
        Head->type = funcType;
        Head->next = NULL;
        Head->params = NULL;
        Head->vars = NULL;

        addFuncParams(atual, Head);

        return;
    }

    // procura o último elemento da lista
    while (aux->next)
        aux = aux->next;

    aux->next = (globalTable *)malloc(sizeof(globalTable));
    aux = aux->next;

    if (strcmp(funcType, "FuncParams") == 0 || strcmp(funcType, "funcParams") == 0)
    {
        funcType = "none";
    }

    aux->func = 1;
    aux->declared = 0;
    aux->name = funcName;
    aux->type = funcType;
    aux->next = NULL;
    aux->params = NULL;
    aux->vars = NULL;

    addFuncParams(atual, aux);

    return;
}

void addFuncParams(no_ast *atual, globalTable *func)
{
    no_ast *nodeAux = atual->filho->filho; // FuncDecl->FuncHeader->Id
    nodeAux = nodeAux->irmao;

    // procura por "FuncParams"
    if (strcmp(nodeAux->tipo, "FuncParams") != 0)
        nodeAux = nodeAux->irmao;

    // verifica se existem parâmetros
    if (!nodeAux->filho)
        return;

    nodeAux = nodeAux->filho; // FuncParams->ParamDecl

    funcParams *paramsHead = NULL; // guarda a localizacao do primeiro parametro
    funcParams *aux = NULL;        // usado para verificar se já existe o parametro
    funcParams *aux1 = NULL;       // usado para criar os elementos da lista ligada
    int flag;                      // usado para sinalizar repetição de parametros

    // percorre todos os "ParamDecl"
    while (nodeAux)
    {
        flag = 0;
        aux = paramsHead;

        while (aux)
        {
            // ParamDecl->Type->Id(valor)
            if (strcmp(aux->name, nodeAux->filho->irmao->valor) == 0)
            {
                flag = 1;
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Symbol %s already defined\n", nodeAux->filho->irmao->line, nodeAux->filho->irmao->column, nodeAux->filho->irmao->valor);
                //printf("Line %d, column %d: Symbol %s already defined\n", nodeAux->filho->irmao->line, nodeAux->filho->irmao->column, nodeAux->filho->irmao->valor);
                addErro(str, nodeAux->filho->irmao->line, nodeAux->filho->irmao->column);
                semanticError = 1;
            }
            aux = aux->next;
        }

        if (!flag)
        {
            if (!aux1)
            {
                aux1 = (funcParams *)malloc(sizeof(funcParams));

                aux1->name = (char *)malloc(sizeof(char) * strlen(nodeAux->filho->irmao->valor) + 1);
                strcpy(aux1->name, nodeAux->filho->irmao->valor); // ParamDecl->Type->Id(valor)
                aux1->type = (char *)malloc(sizeof(char) * strlen(nodeAux->filho->tipo) + 1);
                strcpy(aux1->type, nodeAux->filho->tipo); // ParamDecl->Type
                aux1->type[0] = tolower(aux1->type[0]);
                aux1->next = NULL;

                paramsHead = aux1; // guarda o primeiro elemento da lista
            }
            else
            {
                aux1->next = (funcParams *)malloc(sizeof(funcParams));
                aux1 = aux1->next;

                aux1->name = (char *)malloc(sizeof(char) * strlen(nodeAux->filho->irmao->valor) + 1);
                strcpy(aux1->name, nodeAux->filho->irmao->valor); // ParamDecl->Type->Id(valor)
                aux1->type = (char *)malloc(sizeof(char) * strlen(nodeAux->filho->tipo) + 1);
                strcpy(aux1->type, nodeAux->filho->tipo); // ParamDecl->Type
                aux1->type[0] = tolower(aux1->type[0]);
                aux1->next = NULL;
            }
        }

        nodeAux = nodeAux->irmao;
    }

    func->params = paramsHead;
}

void addGlobalVar(no_ast *atual)
{
    char *varName = (char *)malloc(sizeof(char) * strlen(atual->filho->irmao->valor) + 1);
    strcpy(varName, atual->filho->irmao->valor); // VarDecl->Type->Id(nome)

    char *varType = (char *)malloc(sizeof(char) * strlen(atual->filho->tipo) + 1);
    strcpy(varType, atual->filho->tipo); // VarDecl->Type
    varType[0] = tolower(varType[0]);

    // verifica se já existe uma variavel global com o mesmo nome
    if (existsGlobal(varName, atual->filho->irmao))
    {
        return;
    }

    globalTable *aux = (globalTable *)malloc(sizeof(globalTable));

    if (!Head)
    {
        // primeiro elemento da lista global
        Head = (globalTable *)malloc(sizeof(globalTable));

        Head->func = 0;
        Head->name = varName;
        Head->type = varType;
        Head->next = NULL;

        return;
    }

    aux = Head;

    // procura o fim da lista global
    while (aux->next)
    {
        aux = aux->next;
    }

    aux->next = (globalTable *)malloc(sizeof(globalTable));
    aux = aux->next;

    aux->func = 0;
    aux->name = varName;
    aux->type = varType;
    aux->next = NULL;

    return;
}

int existsGlobal(char *name, no_ast *atual)
{
    globalTable *aux = Head;

    while (aux)
    {
        // percorre a lista global e compara os nomes
        if (strcmp(name, aux->name) == 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Symbol %s already defined\n", atual->line, atual->column, name);
            //printf("Line %d, column %d: Symbol %s already defined\n", atual->line, atual->column, name);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
            return 1;
        }

        aux = aux->next;
    }
    return 0;
}

void addFuncLocal(no_ast *atual)
{
    no_ast *nodeAux = atual->filho->irmao->filho;                    // FuncDecl->FuncHeader->FuncBody->VarDecl, Return ...
    globalTable *funcP = getFuncPointer(atual->filho->filho->valor); // recebe o ponteiro da funçao na lista global

    // verifica se existe a funcao na lista global e ainda nao foi declarada
    if (funcP)
    {
        // percorre as operações de uma função
        while (nodeAux)
        {
            if (strcmp(nodeAux->tipo, "VarDecl") == 0)
            {
                // se for uma declaracao de uma variavel
                addFuncLocalVar(nodeAux, funcP);
            }
            else if (strcmp(nodeAux->tipo, "NULL") != 0)
            {
                // se for outra coisa como um "Assign", "Return" ...
                // anota
                anotaAst(nodeAux, funcP);
            }

            nodeAux = nodeAux->irmao;
        }
    }
}

globalTable *getFuncPointer(char *name)
{
    globalTable *aux = Head;

    while (aux)
    {
        if (strcmp(aux->name, name) == 0 && aux->func)
        {
            // verifica se a função já foi declarada
            if (!aux->declared)
            {
                aux->declared = 1;
                return aux;
            }

            return NULL;
        }

        aux = aux->next;
    }

    return NULL;
}

void addFuncLocalVar(no_ast *atual, globalTable *func)
{
    no_ast *aux = atual->filho->irmao; // VarDecl->Type->Id
    funcParams *paramsAux = func->params;

    // verifica se a variavel existe nos parametros da funcao
    while (paramsAux)
    {
        if (strcmp(paramsAux->name, aux->valor) == 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Symbol %s already defined\n", aux->line, aux->column, aux->valor);
            //printf("Line %d, column %d: Symbol %s already defined\n", aux->line, aux->column, aux->valor);
            addErro(str, aux->line, aux->column);
            semanticError = 1;
            return;
        }

        paramsAux = paramsAux->next;
    }

    funcVars *varsAux = func->vars;

    if (!varsAux)
    {
        // primeiro elemento da lista ligada
        varsAux = (funcVars *)malloc(sizeof(funcVars));

        varsAux->name = (char *)malloc(sizeof(char) * strlen(aux->valor) + 1);
        strcpy(varsAux->name, aux->valor);
        varsAux->type = (char *)malloc(sizeof(char) * strlen(atual->filho->tipo) + 1);
        strcpy(varsAux->type, atual->filho->tipo);
        varsAux->type[0] = tolower(varsAux->type[0]);
        varsAux->next = NULL;

        func->vars = varsAux;

        // verifica se a variavel e utilizada
        if (checkLocalVarUsed(atual->irmao, varsAux->name) == 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Symbol %s declared but never used\n", aux->line, aux->column, varsAux->name);
            //printf("Line %d, column %d: Symbol %s declared but never used\n", aux->line, aux->column, varsAux->name);
            addErro(str, aux->line, aux->column);
            semanticError = 1;
        }

        return;
    }

    // verifica se ja existe a variavel local ao mesmo tempo que percorre a lista até ao fim
    while (varsAux)
    {
        if (strcmp(varsAux->name, aux->valor) == 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Symbol %s already defined\n", aux->line, aux->column, aux->valor);
            //printf("Line %d, column %d: Symbol %s already defined\n", aux->line, aux->column, aux->valor);
            addErro(str, aux->line, aux->column);
            semanticError = 1;
            return;
        }

        if (!varsAux->next)
            break;

        varsAux = varsAux->next;
    }

    varsAux->next = (funcVars *)malloc(sizeof(funcVars));
    varsAux = varsAux->next;

    varsAux->name = (char *)malloc(sizeof(char) * strlen(aux->valor) + 1);
    strcpy(varsAux->name, aux->valor);
    varsAux->type = (char *)malloc(sizeof(char) * strlen(atual->filho->tipo) + 1);
    strcpy(varsAux->type, atual->filho->tipo);
    varsAux->type[0] = tolower(varsAux->type[0]);
    varsAux->next = NULL;

    // verifica se a variavel e utilizada
    if (checkLocalVarUsed(atual->irmao, varsAux->name) == 0)
    {
        char *str = (char *) malloc(516);
        sprintf(str, "Line %d, column %d: Symbol %s declared but never used\n", aux->line, aux->column, varsAux->name);
        //printf("Line %d, column %d: Symbol %s declared but never used\n", aux->line, aux->column, varsAux->name);
        addErro(str, aux->line, aux->column);
        semanticError = 1;
    }
}

int checkLocalVarUsed(no_ast *atual, char *name)
{
    // se chegar ao fim
    if (!atual)
        return 0;

    // procura a variavel em tudo menos em "VarDecl" porque não tem sentido
    if (strcmp(atual->tipo, "VarDecl") == 0)
        return checkLocalVarUsed(atual->irmao, name);

    // enquanto não encontra um "Id" percorre a estrutura
    if (strcmp(atual->tipo, "Id") != 0)
        return checkLocalVarUsed(atual->filho, name) || checkLocalVarUsed(atual->irmao, name);

    // quando encontrar um "Id"
    // se nao for igual ao nome da variavel continua a procurar
    if (strcmp(atual->valor, name) != 0)
        return checkLocalVarUsed(atual->filho, name) || checkLocalVarUsed(atual->irmao, name);

    // se encontrar um "Id" e um nome correspondente retorna 1
    return 1;
}

char *anotaAst(no_ast *atual, globalTable *func)
{
    if (strcmp(atual->tipo, "Print") == 0)
    {
        char *tipo = anotaAst(atual->filho, func);

        if(strcmp(tipo, "undef") == 0){
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Incompatible type undef in fmt.Println statement\n", atual->line, atual->column);
            //printf("Line %d, column %d: Incompatible type undef in fmt.Println statement\n", atual->line, atual->column);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
        }
    }

    else if (strcmp(atual->tipo, "Assign") == 0)
    {
        char *tipo1 = anotaAst(atual->filho, func);
        char *tipo2 = anotaAst(atual->filho->irmao, func);

        // se uma das variaveis  ou ambas não tiverem tipo
        if (strcmp(tipo1, "undef") == 0 || strcmp(tipo2, "undef") == 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Operator = cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            //printf("Line %d, column %d: Operator = cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
            addNota(atual, "undef");
            return "undef";
        }

        // se as duas variáveis não tiverem o mesmo tipo
        if (strcmp(tipo1, tipo2) != 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Operator = cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            //printf("Line %d, column %d: Operator = cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
            addNota(atual, tipo1);
            return tipo1;
        }

        // sao as duas do mesmo tipo logo não há erros
        addNota(atual, tipo1);
        return tipo1;
    }

    else if (strcmp(atual->tipo, "ParseArgs") == 0)
    {
        char *tipo1 = anotaAst(atual->filho, func);
        char *tipo2 = anotaAst(atual->filho->irmao, func);

        // "Parse Args" só funciona com dois int's
        if (strcmp(tipo1, "int") != 0 || strcmp(tipo2, "int") != 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Operator strconv.Atoi cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            //printf("Line %d, column %d: Operator strconv.Atoi cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
            addNota(atual, "undef");
            return "undef";
        }

        addNota(atual, "int");
        return "int";
    }

    else if (strcmp(atual->tipo, "Return") == 0)
    {
        // verifica se o return tem filho
        if (atual->filho)
        {
            char *tipo = anotaAst(atual->filho, func);

            // verifica se o tipo do return corresponde ao da função
            if (strcmp(tipo, func->type) != 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Incompatible type %s in return statement\n", atual->filho->line, atual->filho->column, tipo);
                //printf("Line %d, column %d: Incompatible type %s in return statement\n", atual->filho->line, atual->filho->column, tipo);
                addErro(str, atual->filho->line, atual->filho->column);
                semanticError = 1;
            }
        }
        else
        {
            // verifica se os returns coincidem
            if (strcmp("none", func->type) != 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Incompatible type none in return statement\n", atual->line, atual->column);
                //printf("Line %d, column %d: Incompatible type none in return statement\n", atual->line, atual->column);
                addErro(str, atual->line, atual->column);
                semanticError = 1;
            }
        }
    }

    else if (strcmp(atual->tipo, "Not") == 0)
    {
        char *tipo = anotaAst(atual->filho, func);

        if (strcmp(tipo, "bool") != 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Operator ! cannot be applied to type %s\n", atual->line, atual->column, tipo);
            //printf("Line %d, column %d: Operator ! cannot be applied to type %s\n", atual->line, atual->column, tipo);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
        }

        addNota(atual, tipo);
        return tipo;
    }

    else if (strcmp(atual->tipo, "Minus") == 0 || strcmp(atual->tipo, "Plus") == 0)
    {
        char *tipo = anotaAst(atual->filho, func);

        // se for do tipo "StrLit" ou "String"
        if (strcmp(tipo, "int") != 0 && strcmp(tipo, "float32") != 0)
        {
            if (strcmp(atual->tipo, "Minus") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator - cannot be applied to type %s\n", atual->line, atual->column, tipo);
                //printf("Line %d, column %d: Operator - cannot be applied to type %s\n", atual->line, atual->column, tipo);
                addErro(str, atual->line, atual->column);
                semanticError = 1;
                addNota(atual, "undef");
                return "undef";
            }
            else if (strcmp(atual->tipo, "Plus") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator + cannot be applied to type %s\n", atual->line, atual->column, tipo);
                //printf("Line %d, column %d: Operator + cannot be applied to type %s\n", atual->line, atual->column, tipo);
                addErro(str, atual->line, atual->column);
                semanticError = 1;
                addNota(atual, "undef");
                return "undef";
            }
        }

        addNota(atual, tipo);
        return tipo;
    }

    else if (strcmp(atual->tipo, "If") == 0)
    {
        char *tipo = anotaAst(atual->filho, func);

        if (strcmp(tipo, "bool") != 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Incompatible type %s in if statement\n", atual->filho->line, atual->filho->column, tipo);
            //printf("Line %d, column %d: Incompatible type %s in if statement\n", atual->filho->line, atual->filho->column, tipo);
            addErro(str, atual->filho->line, atual->filho->column);
            semanticError = 1;
        }

        // chama o "Block" do if para anotar
        anotaAst(atual->filho->irmao, func);

        // chama o "Block" do else apesar de poder estar vazio
        anotaAst(atual->filho->irmao->irmao, func);
    }

    else if (strcmp(atual->tipo, "For") == 0)
    {
        char *tipo = anotaAst(atual->filho, func);

        if (strcmp(tipo, "undef") == 0)
            return "undef";

        if (strcmp(tipo, "bool") != 0)
        {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Incompatible type %s in for statement\n", atual->filho->line, atual->filho->column, tipo);
            //printf("Line %d, column %d: Incompatible type %s in for statement\n", atual->filho->line, atual->filho->column, tipo);
            addErro(str, atual->filho->line, atual->filho->column);
            semanticError = 1;
        }

        // chama o "Block" para anotar
        if(atual->filho->irmao)
            anotaAst(atual->filho->irmao, func);
    }

    else if (strcmp(atual->tipo, "Call") == 0)
    {
        char *tipo = addNotaFunc(atual->filho, func);

        addNota(atual, tipo);
        return tipo;
    }

    else if (strcmp(atual->tipo, "Eq") == 0 || strcmp(atual->tipo, "Lt") == 0 || strcmp(atual->tipo, "Gt") == 0 || strcmp(atual->tipo, "Ne") == 0 || strcmp(atual->tipo, "Le") == 0 || strcmp(atual->tipo, "Ge") == 0)
    {
        char *tipo1 = anotaAst(atual->filho, func);
        char *tipo2 = anotaAst(atual->filho->irmao, func);

        if (strcmp(tipo1, "bool") == 0 && strcmp(tipo2, "bool") == 0)
        {
            if (strcmp(atual->tipo, "Lt") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator < cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator < cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Gt") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator > cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator > cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Le") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator <= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator <= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Ge") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator >= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator >= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            addNota(atual, tipo1);
        }
        else if ((strcmp(tipo1, "int") == 0 && strcmp(tipo2, "int") == 0) || (strcmp(tipo1, "float32") == 0 && strcmp(tipo2, "float32") == 0) || (strcmp(tipo1, "string") == 0 && strcmp(tipo2, "string") == 0))
        {
            addNota(atual, "bool");
        }
        else
        {
            if (strcmp(atual->tipo, "Lt") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator < cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator < cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Gt") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator > cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator > cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Le") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator <= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator <= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Ge") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator >= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator >= cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Ne") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator != cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator != cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if (strcmp(atual->tipo, "Eq") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator == cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator == cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            addNota(atual, tipo1);
        }

        return "bool";
    }

    else if (strcmp(atual->tipo, "Add") == 0 || strcmp(atual->tipo, "Sub") == 0 || strcmp(atual->tipo, "Mul") == 0 || strcmp(atual->tipo, "Div") == 0 || strcmp(atual->tipo, "Mod") == 0)
    {
        char *tipo1 = anotaAst(atual->filho, func);
        char *tipo2 = anotaAst(atual->filho->irmao, func);
        
        if((strcmp(tipo1, tipo2) != 0) || (strcmp(tipo1, "bool") == 0 && strcmp(tipo2, "bool") == 0) || (strcmp(tipo1, "undef") == 0 && strcmp(tipo2, "undef") == 0)) {
            if(strcmp(atual->tipo, "Add") == 0){
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator + cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator + cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if(strcmp(atual->tipo, "Sub") == 0){
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator - cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator - cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if(strcmp(atual->tipo, "Mul") == 0){
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator * cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator * cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if(strcmp(atual->tipo, "Div") == 0){
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator / cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator / cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            else if(strcmp(atual->tipo, "Mod") == 0){
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator %% cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator %% cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
            }
            addNota(atual, "undef");
            return "undef";
        }

        if(strcmp(tipo1, "float32") == 0 && strcmp(atual->tipo, "Mod") == 0) {
            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Operator %% cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            //printf("Line %d, column %d: Operator %% cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
            addErro(str, atual->line, atual->column);
            addNota(atual, "undef");
            return "undef";
        }

        addNota(atual, tipo1);
        return tipo1;
    }

    else if (strcmp(atual->tipo, "And") == 0 || strcmp(atual->tipo, "Or") == 0)
    {
        char *tipo1 = anotaAst(atual->filho, func);
        char *tipo2 = anotaAst(atual->filho->irmao, func);

        if (strcmp(tipo1, "bool") != 0 || strcmp(tipo2, "bool") != 0)
        {
            if (strcmp(atual->tipo, "And") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator && cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator && cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
                semanticError = 1;
            }
            else if (strcmp(atual->tipo, "Or") == 0)
            {
                char *str = (char *) malloc(516);
                sprintf(str, "Line %d, column %d: Operator || cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                //printf("Line %d, column %d: Operator || cannot be applied to types %s, %s\n", atual->line, atual->column, tipo1, tipo2);
                addErro(str, atual->line, atual->column);
                semanticError = 1;
            }
        }

        addNota(atual, "bool");
        return "bool";
    }

    else if (strcmp(atual->tipo, "Block") == 0)
    {
        // se tiver filho
        if (atual->filho)
        {
            no_ast *aux = atual->filho;

            // percorre o block inteiro
            while (aux)
            {
                anotaAst(aux, func);
                aux = aux->irmao;
            }
        }
    }

    else if (strcmp(atual->tipo, "StrLit") == 0)
    {
        addNota(atual, "string");
        return "string";
    }

    else if (strcmp(atual->tipo, "RealLit") == 0)
    {
        addNota(atual, "float32");
        return "float32";
    }

    else if (strcmp(atual->tipo, "IntLit") == 0)
    {
        addNota(atual, "int");
        return "int";
    }

    else if (strcmp(atual->tipo, "Id") == 0)
    {
        char *r = addNotaId(atual, func);
        return r;
    }

    return "undef";
}

char *addNotaId(no_ast *atual, globalTable *func)
{
    // procura nome nas variaveis locais
    funcVars *varsAux = func->vars;

    while (varsAux)
    {
        if (strcmp(atual->valor, varsAux->name) == 0)
        {
            addNota(atual, varsAux->type);
            return varsAux->type;
        }

        varsAux = varsAux->next;
    }

    // procura nome nos parametros da funcao
    funcParams *paramsAux = func->params;

    while (paramsAux)
    {
        if (strcmp(atual->valor, paramsAux->name) == 0)
        {
            addNota(atual, paramsAux->type);
            return paramsAux->type;
        }

        paramsAux = paramsAux->next;
    }

    // procura nome nas variaveis globais
    globalTable *aux = Head;

    while (aux)
    {
        if (strcmp(atual->valor, aux->name) == 0 && !aux->func)
        {
            addNota(atual, aux->type);
            return aux->type;
        }

        aux = aux->next;
    }

    // não encontrou a variável em lado nenhum
    char *str = (char *) malloc(516);
    sprintf(str, "Line %d, column %d: Cannot find symbol %s\n", atual->line, atual->column, atual->valor);
    //printf("Line %d, column %d: Cannot find symbol %s\n", atual->line, atual->column, atual->valor);
    addErro(str, atual->line, atual->column);
    semanticError = 1;
    addNota(atual, "undef");
    return "undef";
}

char *addNotaFunc(no_ast *atual, globalTable *func)
{
    // procura a função invocada na lista global
    globalTable *aux = Head;

    while (aux)
    {
        if (aux->func && strcmp(aux->name, atual->valor) == 0)
        {
            // se encontrar a função
            no_ast *noAux = atual->irmao;
            funcParams *paramsAux = aux->params;

            int given = 0, needed = 0; // usados para verificar se o numero de parametros fornecidos e precisos coincidem
            int error = 0;             // usado para ver se há erros nos tipos
            char s[128];
            strcpy(s, "(");

            while (noAux || paramsAux)
            {
                char *tipo;

                // adiciona os tipos à string
                if (noAux)
                {
                    tipo = anotaAst(noAux, func);

                    if (strcmp(s, "(") == 0)
                    {
                        strcat(s, tipo);
                    }
                    else
                    {
                        strcat(s, ",");
                        strcat(s, tipo);
                    }
                }
                // verifica se os tipos dos dados fornecidos estão corretos
                if (noAux && paramsAux && strcmp(tipo, paramsAux->type) != 0)
                {
                    error = 1;
                    if(strcmp(tipo, "int") == 0 && strcmp(paramsAux->type, "bool") == 0)
                        error = 0;
                }
                // conta o numero de parametros dados
                if (noAux)
                {
                    given++;
                    noAux = noAux->irmao;
                }
                // conta o numero de parametros precisos
                if (paramsAux)
                {
                    needed++;
                    paramsAux = paramsAux->next;
                }
            }
            // termina a string dos tipos
            strcat(s, ")");

            // se nao houve erros
            if (given == needed && error == 0)
            {
                addNota(atual, s);
                return aux->type;
            }

            char *str = (char *) malloc(516);
            sprintf(str, "Line %d, column %d: Cannot find symbol %s%s\n", atual->line, atual->column, atual->valor, s);
            //printf("Line %d, column %d: Cannot find symbol %s%s\n", atual->line, atual->column, atual->valor, s);
            addErro(str, atual->line, atual->column);
            semanticError = 1;
            addNota(atual, "undef");
            return "undef";
        }

        aux = aux->next;
    }
    // se a funcao nao existir tem que se anotar a arvore na mesma
    no_ast *noAux = atual->irmao;
    char s[128];
    strcpy(s, "(");

    while (noAux)
    {
        char *tipo;

        // adiciona os tipos à string
        if (noAux)
        {
            tipo = anotaAst(noAux, func);

            if (strcmp(s, "(") == 0)
            {
                strcat(s, tipo);
            }
            else
            {
                strcat(s, ",");
                strcat(s, tipo);
            }
        }

        noAux = noAux->irmao;
    }
    // termina a string dos tipos
    strcat(s, ")");

    char *str = (char *) malloc(516);
    sprintf(str, "Line %d, column %d: Cannot find symbol %s%s\n", atual->line, atual->column, atual->valor, s);
    //printf("Line %d, column %d: Cannot find symbol %s%s\n", atual->line, atual->column, atual->valor, s);
    addErro(str, atual->line, atual->column);
    semanticError = 1;
    addNota(atual, "undef");
    return "undef";
}

void addErro(char *s, int linha, int column)
{
    if (!Err)
    {
        Err = (erros *)malloc(sizeof(erros));
        Err->linha = linha;
        Err->column = column;
        Err->erro = s;
        Err->next = NULL;
        return;
    }

    erros *new = (erros *)malloc(sizeof(erros));
    new->linha = linha;
    new->column = column;
    new->erro = s;
    new->next = NULL;

    erros *aux = Err;
    while (aux->next)
    {
        if (aux->next->linha > linha || (aux->next->linha == linha && aux->next->column >= column))
            break;
        aux = aux->next;
    }
    new->next = aux->next;
    aux->next = new;
}

void printErros()
{
    erros *aux = Err;

    while (aux)
    {
        printf("%s", aux->erro);
        aux = aux->next;
    }
}
