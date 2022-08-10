//Marco da Cruz Pais Nº2019218981
//Simão Carvalho Monteiro Nº2019215412

#include <stdio.h>
#include "raceSimulator.h"

int main() {
    FILE *config = fopen("config.txt", "r");
    raceSimulator(config);
    return 0;
}
