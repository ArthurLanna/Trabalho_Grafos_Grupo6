#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>
#include <filesystem>
#include <string>
#include "Guloso.h"
#include <random>
#include <chrono>
#include <cmath>

using namespace std;
class Gerenciador {
public:

    //Funções que já existiam
    static void comandos(Grafo* grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);

    //Função auxiliar criada
    static void criaPastaSaida();
    //constante de controle da pasta de saida
    static const string pastaSaida;
    //Função auxiliar criada para a geração de valores aleatórios entre 0 e 1
    static double geraNumeros();

    //Função auxiliar criada para simplificação da escrita do codigo
    static void teste_gulosoRandomizado(Grafo* grafo, double alpha);
    
};


#endif //GERENCIADOR_H
