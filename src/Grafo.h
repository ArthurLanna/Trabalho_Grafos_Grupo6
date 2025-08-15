#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <filesystem>


using namespace std;
class Grafo {
public:
    //Funções criadas para auxiliar
    Grafo(int argc, char *argv[]);
    Grafo();
    ~Grafo();
    void leArquivo(int argc, char *argv[]);
    void criaListaAdj();
    No* buscaOuCriaNo(char id, int pesoVertice);
    void imprimeListaAdjacencia();
    int obterPesoVertice(char id_vertice);
    void buscaEmProfundidade(char id_atual, set<char>& visitados);
    void buscaEmProfundidadeReversa(char id_atual, set<char>& visitados);
    No* buscaNo(char id);
    bool verificaPonderadoAresta();
    bool verificaPonderadoVertice();
    bool verificaDirecionado();
    void imprimeListaAdjacenciaEmArquivo(FILE *a);
    void bep_para_arvore(char id_atual, set<char>& visitados, Grafo* arvore);
    void calcularMetricasDeDistancia();
    int getOrdem();
    vector<No*> getListaAdj();

    //constante de indicação da pasta de entrada
    static const string pastaEntrada;
    

    //Funções já existentes no projeto
    vector<char> fecho_transitivo_direto(char id_no); // a
    vector<char> fecho_transitivo_indireto(char id_no); // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4

private:
    //variáveis que já existiam 
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;

    //variáveis criadas para auxiliar
    char flags[4]= {0};
    vector<string> nos;
    vector<string> arestas;
    bool metricas_calculadas = false;
    int m_raio;
    int m_diametro;
    map<char, int> m_excentricidades;
    vector<char> m_centro;
    vector<char> m_periferia;
};



#endif //GRAFO_H
