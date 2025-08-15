#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No(bool ponderado, int peso, char id);
    ~No();
    void setNo(bool ponderado, int peso, char id);
    void adicionaVizinho(Aresta* a);
    char getId();
    void setPeso(int pesoVertice);
    int getPeso();
    vector<Aresta*> getArestasAdj();

    char id;
    int peso;
    vector<Aresta*> arestasAdj;

};



#endif //NO_H
