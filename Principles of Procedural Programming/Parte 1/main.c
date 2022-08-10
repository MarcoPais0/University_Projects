//Marco da Cruz Pais Nº2019218981
#include "aux_func.h"

int main() {
    char filename[MAX_WORD];
    //pede o nome do ficheiro ao utilizador
    get_filename(filename);
    //lê o texto e cria o ficheiro binário com a informação necessária
    analyze_text(filename);
    return 0;
}
