#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <vector>
#include <set>
#include <map>

using namespace std;

class Guloso {

    public:
        //Método para encontrar o conjunto dominante de peso minimo com Algorítmo Guloso
        static set<char> encontraMWDS_Guloso(Grafo* grafo);

        //Método para calcular o peso do conjunto dominante de peso minimo
        static int calculaPesoConjunto(set<char> conjuntoDominante, Grafo* grafo);

        //Método para encontrar o conjunto dominante de peso minimo com Algorítmo Guloso Randomizado
        static set<char> encontraMWDS_GulosoRandomizado(Grafo* grafo, double alpha);

        //Método para encontrar o conjunto dominante de peso minimo com Algorítmo Guloso Randomizado Reativo
        static set<char> encontraMWDS_GulosoRandomizadoReativo(Grafo* grafo, const vector<double> alphas_possiveis, int num_iteracao_alpha, int num_blocos_aprendizagem);
    
};

#endif //Guloso.h
