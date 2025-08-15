#include <iostream>
#include "Aresta.h"
#include "Grafo.h"
#include "No.h"

using namespace std;

Aresta::Aresta(bool direcionado, bool ponderado, char id_na_fonte, char id_no_alvo, int peso ){
    setAresta(direcionado,ponderado,id_na_fonte,id_no_alvo,peso);
}

char Aresta::getIdNoAlvo(){
    return this->id_no_alvo;
}

char Aresta::getIdNoFonte(){
    return this->id_na_fonte;
}

Aresta::~Aresta(){

}

void Aresta::setAresta(bool direcionado, bool ponderado, char id_na_fonte, char id_no_alvo, int peso){
    this->id_na_fonte = id_na_fonte;
    this->id_no_alvo = id_no_alvo;
    this->peso = peso;
}

int Aresta::getPeso(){
    return this->peso;
}