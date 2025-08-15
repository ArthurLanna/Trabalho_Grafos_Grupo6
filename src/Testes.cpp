#include "Testes.h"
#include <limits>

using namespace std;

set<char> Testes::testa_Guloso(Grafo* grafo){
    int melhorResultado = numeric_limits<int>::max();
    set<char> conjuntoDominante;
    set<char> melhorConjunto;
    for(int i = 0; i < 10; i++){
        conjuntoDominante = Guloso::encontraMWDS_Guloso(grafo);
        int aux = Guloso::calculaPesoConjunto(conjuntoDominante, grafo);
        if(aux < melhorResultado){
            melhorResultado = aux;
            melhorConjunto = conjuntoDominante;
        }
    }
    return melhorConjunto;
}

set<char> Testes::testa_GulosoRandomizado(Grafo* grafo, double alpha){
    int melhorResultado = numeric_limits<int>::max();
    set<char> conjuntoDominante;
    set<char> melhorConjunto;
    for(int i = 0; i < 10; i++){
        conjuntoDominante = Guloso::encontraMWDS_GulosoRandomizado(grafo, alpha);
        int aux = Guloso::calculaPesoConjunto(conjuntoDominante, grafo);
        if(aux < melhorResultado){
            melhorResultado = aux;
            melhorConjunto = conjuntoDominante;
        }
    }
    return melhorConjunto;
}

set<char> Testes::testa_GulosoReativo(Grafo* grafo, const vector<double> alphas_possiveis, int num_iteracao_alpha, int num_blocos_aprendizagem){
    int melhorResultado = numeric_limits<int>::max();
    set<char> conjuntoDominante;
    set<char> melhorConjunto;
    for(int i = 0; i < 10; i++){
        conjuntoDominante = Guloso::encontraMWDS_GulosoRandomizadoReativo(grafo, alphas_possiveis,num_iteracao_alpha,num_blocos_aprendizagem);
        int aux = Guloso::calculaPesoConjunto(conjuntoDominante, grafo);
        if(aux < melhorResultado){
            melhorResultado = aux;
            melhorConjunto = conjuntoDominante;
        }
    }
    return melhorConjunto;
}