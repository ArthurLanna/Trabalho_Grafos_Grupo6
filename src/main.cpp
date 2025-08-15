#include <iostream>
#include "Gerenciador.h"
#include "Grafo.h"

using namespace std;
int main(int argc, char *argv[])
{
    Grafo *grafo = new Grafo(argc, argv);
    Gerenciador::comandos(grafo);

    return 0;
}
