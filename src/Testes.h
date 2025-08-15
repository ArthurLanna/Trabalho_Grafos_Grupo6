#ifndef TESTES_H
#define TESTES_H

#include "Guloso.h"
#include "Grafo.h"
#include "Gerenciador.h"

using namespace std;
class Testes{
public:
    //Metodo para executar o algoritmo guloso 10 vezes em sequencia e salvar o melhor resultado
    set<char> testa_Guloso(Grafo* grafo);

    //Metodo para executar o algoritimo guloso randomizado 10 vezes em sequencia e salvar o melhor resultado
    set<char> testa_GulosoRandomizado(Grafo* grafo, double alpha);

    //Metodo para executar o algoritmo guloso randomizado reativo 10 vezes em sequencia e salvar o melhor resultado
    set<char> testa_GulosoReativo(Grafo* grafo, const vector<double> alphas_possiveis, int num_iteracao_alpha, int num_blocos_aprendizagem);
    
};

#endif //TESTES_H