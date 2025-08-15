#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta(bool direcionado, bool ponderado, char id_na_fonte, char id_no_alvo, int peso);
    ~Aresta();
    void setAresta(bool direcionado, bool ponderado, char id_na_fonte, char id_no_alvo, int peso);
    char getIdNoAlvo();
    char getIdNoFonte();
    int getPeso();


    char id_na_fonte;
    char id_no_alvo;
    int peso;
    
};



#endif //ARESTA_H
