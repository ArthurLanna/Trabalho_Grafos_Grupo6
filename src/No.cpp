#include<iostream>
#include "No.h"
#include "Aresta.h"
#include "Grafo.h"
#include <vector>
#include <string>

using namespace std;

No::No(bool ponderado, int peso, char id){
    setNo(ponderado, peso, id);
}

No::~No(){
    
}

void No::setNo(bool ponderado, int peso, char id){
    this->peso = peso;
    this->id = id;
}

char No::getId(){
    return this->id;
}

void No::adicionaVizinho(Aresta* a) {
        arestasAdj.push_back(a);
}

void No::setPeso(int pesoVertice){
    peso = pesoVertice;
}

int No::getPeso(){
    return this->peso;
}

vector<Aresta*> No::getArestasAdj(){
    return this->arestasAdj;
}
