/*META 4 - COMPILADORES 2021-2022*/
/*MARCO PAIS Nº 2019218981*/
/*TIAGO OLIVEIRA Nº 2019219068*/

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "struct.h"
    #include "semantics.h"
    #include "code.h"
    #include "y.tab.h"

    #define LINE line++, column = 1;
    #define COLUMN column += yyleng;
    
    int line_com, col_com, line_str, col_str; // Auxiliares para erros
    int flag = 0, flag_escape = 0, _l = 0, _t = 0, line = 1, column = 1; // Flags e contadores
    char *straux; // Guarda a string atual

    extern int syntaxError; // True se existirem erros de compilação
    extern no_ast* root; // No da arvore AST
    extern int semanticError;
%}

STRLIT		\"([^\n\r\\\"]|{ESCAPE})*\" 

NEWLINE             \n|\r|\r\n
ESCAPE              \\f|\\n|\\r|\\t|\\\\|\\\"
NUMBER              [0-9]
OCTAL               0[0-7]*
ILLEGAL_OCTAL       0{NUMBER}*
HEX                 0[xX][0-9A-Fa-f]+
DECIMAL             [1-9]{NUMBER}*
EXPONENT            [eE][+-]?{NUMBER}+
LETTER              [a-zA-Z_]
ID                  {LETTER}({LETTER}|{NUMBER})* 
INTLIT              ({OCTAL}|{HEX}|{DECIMAL})
REALLIT             ({NUMBER}+"."{NUMBER}*{EXPONENT}?)|("."{NUMBER}+{EXPONENT}?)|{NUMBER}+{EXPONENT}
PLUS                "+"
MINUS               "-"
RESERVED            break|default|interface|select|case|defer|go|map|struct|chan|goto|switch|const|fallthrough|range|type|continue|import|{PLUS}{2}|{MINUS}{2}

%x LINE_COMMENT GENERAL_COMMENT STRLIT_ERROR
%%

";"         {if (_l) printf("SEMICOLON\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return SEMICOLON;}}

","         {if (_l) printf("COMMA\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return COMMA;}}
"="         {if (_l) printf("ASSIGN\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return ASSIGN;}}
"*"         {if (_l) printf("STAR\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return STAR;}}
"/"         {if (_l) printf("DIV\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return DIV;}}
{MINUS}     {if (_l) printf("MINUS\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return MINUS;}}
{PLUS}      {if (_l) printf("PLUS\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return PLUS;}}
"=="        {if (_l) printf("EQ\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return EQ;}}
">="        {if (_l) printf("GE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return GE;}}
">"         {if (_l) printf("GT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return GT;}}
"<="        {if (_l) printf("LE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return LE;}}
"<"         {if (_l) printf("LT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return LT;}}
"%"         {if (_l) printf("MOD\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return MOD;}}
"!="        {if (_l) printf("NE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return NE;}}
"!"         {if (_l) printf("NOT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return NOT;}}
"&&"        {if (_l) printf("AND\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return AND;}}
"||"        {if (_l) printf("OR\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return OR;}}

"{"         {if (_l) printf("LBRACE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return LBRACE;}}
"("         {if (_l) printf("LPAR\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return LPAR;}}
"["         {if (_l) printf("LSQ\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return LSQ;}}
"}"         {if (_l) printf("RBRACE\n"); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return RBRACE;}}
")"         {if (_l) printf("RPAR\n"); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return RPAR;}}
"]"         {if (_l) printf("RSQ\n"); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return RSQ;}}
"package"   {if (_l) printf("PACKAGE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return PACKAGE;}}

"return"    {if (_l) printf("RETURN\n"); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return RETURN;}}

"for"       {if (_l) printf("FOR\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return FOR;}}
"if"        {if (_l) printf("IF\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return IF;}}

"else"      {if (_l) printf("ELSE\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return ELSE;}}
"var"       {if (_l) printf("VAR\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return VAR;}}
"int"       {if (_l) printf("INT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return INT;}}
"float32"   {if (_l) printf("FLOAT32\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return FLOAT32;}}
"bool"      {if (_l) printf("BOOL\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return BOOL;}}
"string"    {if (_l) printf("STRING\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return STRING;}}
"fmt.Println"    {if (_l) printf("PRINT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column + 1); return PRINT;}}

"strconv.Atoi"   {if (_l) printf("PARSEINT\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.no = novoNo("NULL", NULL, line, column - yyleng); return PARSEINT;}}

"func"           {if (_l) printf("FUNC\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return FUNC;}}
"os.Args"        {if (_l) printf("CMDARGS\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return CMDARGS;}}
{RESERVED}       {if (_l) printf("RESERVED(%s)\n", yytext); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return RESERVED;}}
"_"         {if (_l) printf("BLANKID\n"); flag = 0; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return BLANKID;}}

{REALLIT}           {if (_l) printf("REALLIT(%s)\n", yytext); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return REALLIT;}}
{INTLIT}            {if (_l) printf("INTLIT(%s)\n", yytext); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return INTLIT;}}
{ILLEGAL_OCTAL}     {printf("Line %d, column %d: invalid octal constant (%s)\n", line, column, yytext); flag = 0; COLUMN;}
{ID}                {if (_l) printf("ID(%s)\n", yytext); flag = 1; COLUMN; if (_t){ straux = yytext; yylval.valor = (char*) strdup(yytext); return ID;}}

{STRLIT}     {flag = 1; if(_l == 1) printf("STRLIT(%s)\n",yytext); COLUMN; yylval.valor = (char*) strdup(yytext); if(_t){ yylval.valor = (char*) strdup(yytext); return STRLIT;}}

\"                                  {BEGIN STRLIT_ERROR; line_str = line; col_str = column; COLUMN; flag = 0;}
<STRLIT_ERROR>\"                    {flag = 1; if(_l == 1) printf("STRLIT(%s)\n",yytext); COLUMN; BEGIN 0;}
<STRLIT_ERROR>{NEWLINE}             {BEGIN 0; printf("Line %d, column %d: unterminated string literal\n", line_str, col_str); LINE;}
<STRLIT_ERROR>{ESCAPE}|[^\\\r\n\"]  {COLUMN;} 
<STRLIT_ERROR>\\[^\n\r\"]|\\|\f|\t  {printf("Line %d, column %d: invalid escape sequence (%s)\n", line_str, column, yytext); COLUMN; flag = 0;}
<STRLIT_ERROR><<EOF>>               {BEGIN 0; printf("Line %d, column %d: unterminated string literal\n", line_str, col_str);} 

"//"                            {BEGIN LINE_COMMENT;}
<LINE_COMMENT>{NEWLINE}         {BEGIN 0; LINE;}
<LINE_COMMENT>.                 {COLUMN;}

"/*"                            {line_com = line; col_com = column; BEGIN GENERAL_COMMENT; COLUMN;}
<GENERAL_COMMENT>{NEWLINE}      {LINE;}
<GENERAL_COMMENT><<EOF>>        {printf("Line %d, column %d: unterminated comment\n", line_com, col_com); yyterminate();}
<GENERAL_COMMENT>.              {COLUMN;}
<GENERAL_COMMENT>"*/"           {BEGIN 0; COLUMN;}

" "         {COLUMN; straux=yytext;}
"\t"        {COLUMN; straux=yytext;}
{NEWLINE}   {LINE; if(flag) {flag = 0; if (_l) printf("SEMICOLON\n"); if (_t){ straux = ""; yylval.valor = (char*) strdup(yytext); return SEMICOLON; }}}
<<EOF>>     {if(flag) {flag = 0; if (_l) printf("SEMICOLON\n"); if (_t) return SEMICOLON; } straux = ""; yylval.valor = (char*) strdup("EOF"); return EOF;}
.           {flag = 0; printf("Line %d, column %d: illegal character (%s)\n", line, column, yytext); COLUMN;}

%%

int main(int argc, char* argv[])
{
    // Verfifica se existem flags a ser lidas
    if (argc > 1) {
        // Flag -l
        if (strcmp(argv[1], "-l") == 0) {
            _l = 1;
            yylex();
        }
        // Flag -t
        if (strcmp(argv[1], "-t") == 0) {
            _t = 1;
            yyparse();
            if (!syntaxError) printAST(root,0);
        }
        // Flag -s
        if (strcmp(argv[1], "-s") == 0) {
            _t = 1;
            yyparse();
            if (!syntaxError){
                criaTabelas(root);
                printTabelas();
                printAST(root,0);
            }
        }
    }
    // No flags
    else {
        // Se não forem introduzidas flags só mostra os erros e gera o código
        _t = 1;
        yyparse();
        criaTabelas(root);
        if(!syntaxError && !semanticError){
            geraCode(root);
        }
    }

    freeAST(root);
    yylex_destroy();    // liberta a memoria alocada pelo scanner interno do lex
    return 0;
}

int yywrap()
{
    return 1;
}
