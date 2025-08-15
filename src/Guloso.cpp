#include <iostream>
#include "Guloso.h"
#include "Grafo.h"
#include "Gerenciador.h"
#include <limits>
#include <random> //Para gerar números pseudo-aleatórios
#include <chrono> //Para gerar sementes para o gerador de números pseudo-aleatórios
#include <utility> //Para utilizar a função 'pair'
#include <cmath>
#include <algorithm>


using namespace std;


set<char> Guloso::encontraMWDS_Guloso(Grafo* grafo){

    set<char> conjuntoDominante; //Conjunto dominante esperado
    set<char> conjuntoDominado; //Conjunto dos vertices que já foram dominados 
    set<char> naoDominados; //Nós que ainda não foram dominados
    //Inicializa o conjunto dos nós não dominados
    for(No* no : grafo->getListaAdj()){
        naoDominados.insert(no->getId());
    }

    //--------------------------[DEBUG]-------------------------
    // cout <<"Fora do While Conjunto Não dominado: ";
    // for(char c : naoDominados){
    //    cout << c << " ";
    // }
    // cout << endl;
    //--------------------------[DEBUG]-------------------------

    //Laço principal do algoritmo Guloso
    while(!naoDominados.empty()){

        char melhor_no_selecionado = '\0';
        double menor_razao = numeric_limits<double>::max();//Utilizando double para razão

        //-----------------------[DEBUG]--------------------------
        // cout <<"Dentro do While Conjunto Não dominado: ";
        // for(char c : naoDominados){
        //     cout << c << " ";
        // }
        // cout << endl;
        //-----------------------[DEBUG]--------------------------

        //Procura entre todos os nós o "melhor" nó para adicionar ao conjunto
        for(No* no_candidato : grafo->getListaAdj()){

            //Redução na chamada da função no_candidato->getId()
            char id_candidato = no_candidato->getId();

            //Ignora nós que já estão no conjunto dominado
            if(conjuntoDominado.count(id_candidato)){
                continue;
            }

            //Calcula quantidade de nós não dominados que este candidato dominaria
            int novos_dominados_candidato = 0;
            //Verifica se o candidato não está dominado
            if(naoDominados.count(id_candidato)){
                novos_dominados_candidato++;
            }
            
            for(Aresta* aresta : no_candidato->getArestasAdj()){

                char vizinhoID = aresta->getIdNoAlvo();
                if(naoDominados.count(vizinhoID)){
                    novos_dominados_candidato++;
                }
            }

            //Obtem peso do candidato
            int peso_candidato = grafo->verificaPonderadoVertice() ? no_candidato->getPeso() : 1; //Peso padrão = 1
            
            double razao = numeric_limits<double>::max();//Inicializa com INF

            //Calcula a razão: peso(v)/número de vértices não dominados que v cobre
            if(novos_dominados_candidato > 0){//evita divisão pro 0
                razao = (double)peso_candidato/novos_dominados_candidato;
                //--------------------------[DEBUG]---------------------------
                // cout << "Nó Candidato: " << no_candidato->getId() << endl;
                // cout << "Razão: " << razao << endl;
                //--------------------------[DEBUG]----------------------------
            }
            //Se novos_dominados for 0, a razão permanece máxima, desfavorecendo a escolha deste candidato

            //Heurística: Escolher o vértice de menor razão
            if(razao < menor_razao){
                menor_razao = razao;
                melhor_no_selecionado = no_candidato->getId();
            }
        }

        if(melhor_no_selecionado == '\0'){
            //Quando não é possível dominar mais nenhum outro nó
            break;
        }

        //adiciona o melhor candidato ao conjunto
        conjuntoDominante.insert(melhor_no_selecionado);
        //adiciona todos os nós já dominados ao conjunto dos nós dominados
        conjuntoDominado.insert(melhor_no_selecionado);
        No* aux = grafo->buscaNo(melhor_no_selecionado);
        if(aux){
            for(Aresta* a : aux->getArestasAdj()){
                conjuntoDominado.insert(a->getIdNoAlvo());
            }
        }

        //Remove o nó dominado e seus vizinhos do conjunto dos não dominados
        set<char> nos_a_remover;
        nos_a_remover.insert(melhor_no_selecionado);//O proprio nó selecionado
        No* obj_selecionado = grafo->buscaNo(melhor_no_selecionado);
        if(obj_selecionado){
            for(Aresta* aresta : obj_selecionado->getArestasAdj()){
                nos_a_remover.insert(aresta->getIdNoAlvo());
            }
        }

        for(char id_a_remover : nos_a_remover){
            naoDominados.erase(id_a_remover);
        }

        //------------------[DEBUG]------------------------
        // cout <<"Final do While Conjunto Não dominado: ";
        // for(char c : naoDominados){
        //     cout << c << " ";
        // }
        // cout << endl;
        //------------------[DEBUG]------------------------
    }
    return conjuntoDominante;
}

int Guloso::calculaPesoConjunto(set<char> conjuntoDominante, Grafo* grafo){

    int peso_conjunto = 0;
    for(char c : conjuntoDominante){
        No* no = grafo->buscaNo(c);
        if(no){
            cout << "Inserindo o peso do no: " << no->getId() << endl;
            cout << "no->getPeso(): " << no->getPeso() << endl;
            peso_conjunto += (grafo->verificaPonderadoVertice() ? no->getPeso() : 1);
        }
    }

    return peso_conjunto;
}

set<char> Guloso::encontraMWDS_GulosoRandomizado(Grafo* grafo, double alpha){

    set<char> conjuntoDominante; //Conjunto dominante esperado
    set<char> conjuntoDominado; //Conjunto dos vertices que já foram dominados 
    set<char> naoDominados; //Nós que ainda não foram dominados

    //Inicialização do conjunto dos nós não dominados
    for(No* no : grafo->getListaAdj()){
        naoDominados.insert(no->getId());
    }

    //Inicialização do gerador de números aleatórios
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    //Laço principal do Algorítmo Guloso Randomizado
    while(!naoDominados.empty()){

        vector<pair<char,double>> nos_candidatos_avaliados; //{id do nó, razao}
        double melhor_razao = numeric_limits<double>::max();
        double pior_razao = 0.0;

        for(No* no_candidato : grafo->getListaAdj()){

            //Redução na chamada da função no_candidato->getId()
            char id_candidato = no_candidato->getId();
            
            //Ignora nós já dominados
            if(conjuntoDominante.count(id_candidato)){
                continue;
            }

            set<char> novos_dominados;

            //Se o proprio candidato ainda não tiver sido dominado ele é considerado
            if(!conjuntoDominado.count(id_candidato)){
                novos_dominados.insert(id_candidato);
            }

            //Considera os vizinhos ainda não dominados
            for(Aresta* a : no_candidato->getArestasAdj()){
                char vizinho = a->getIdNoAlvo();
                if(!conjuntoDominado.count(vizinho)){
                    novos_dominados.insert(vizinho);
                }
            }

            //Descarta candidatos desnecessários
            if(novos_dominados.empty()){
                continue;
            }

            //Calculo da razão para criterio de seleção dos candidatos
            double razao = (double)no_candidato->getPeso()/novos_dominados.size();
            nos_candidatos_avaliados.push_back({id_candidato, razao});

            if(razao < melhor_razao){
                melhor_razao = razao;
            }
            if(razao > pior_razao){
                pior_razao = razao;
            }

        }
        

        if(nos_candidatos_avaliados.empty()){
            break;
        }

        //Calcula o limite RCL
        double limite_razao = melhor_razao + alpha * (pior_razao - melhor_razao);

        //Montagem da lista de candidatos
        vector<char> rcl ;
        for(const auto& [id, razao] : nos_candidatos_avaliados){
            if(razao <= limite_razao){
                rcl.push_back(id);
            }
        }

        if(rcl.empty()){
            break;
        }

        //Seleciona aleatóriamente um nó da lista de candidatos
        uniform_int_distribution<> distrib(0,rcl.size() - 1);
        char selecionado = rcl[distrib(rng)];

        //Atualiza o conjunto dominante
        conjuntoDominante.insert(selecionado);

        //Marca como dominado o nó selecionado
        conjuntoDominado.insert(selecionado);
        naoDominados.erase(selecionado);

        //Marca os vizinhos como dominados
        No* obj = grafo->buscaNo(selecionado);
        if(obj){
            for(Aresta* a : obj->getArestasAdj()){
                char vizinho = a->getIdNoAlvo();
                conjuntoDominado.insert(vizinho);
                naoDominados.erase(vizinho);
            }
        }

    }

    return conjuntoDominante;
}

 set<char> Guloso::encontraMWDS_GulosoRandomizadoReativo(Grafo* grafo, const vector<double> alphas_possiveis, int num_iteracao_alpha, int num_blocos_aprendizagem){

    if(alphas_possiveis.empty()){
        return {};
    }

    //Inicialização do gerador de números
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    //Estruturas de controle do desempenho de cada alpha
    map<double,double> soma_pesos_alpha; //Calcula os pesos das soluções de cada alpha
    map<double, int> contador_alpha; //Quantidade de vezes que cada alpha foi utilizado

    //Inicialização dos contadores
    for(double alpha : alphas_possiveis){
        soma_pesos_alpha[alpha] = 0.0;
        contador_alpha[alpha] = 0;
    }

    set<char> melhor_solucao_geral;
    int menor_peso_melhor_solucao = numeric_limits<int>::max();
    int total_iteracoes = num_iteracao_alpha * num_blocos_aprendizagem;

    for(int iteracao = 0; iteracao < total_iteracoes; iteracao++){

        //Calculo da probabilidade de seleção de cada alpha
        vector<double> prob_selecao;
        double soma_total = 0.0;

        for(double alpha : alphas_possiveis){
            if(contador_alpha[alpha] == 0){ //Se nunca foi utilizado, atribui valor de probabilidade alta, ou um valor fixo
                prob_selecao.push_back(1.0);
            }
            else {
                double media = soma_pesos_alpha[alpha] / contador_alpha[alpha];
                if(media == 0) media = 1.0; //Evita divisão por 0
                prob_selecao.push_back(1.0 / media); //Inverso do peso medio
            }
            soma_total += prob_selecao.back();
        }

        //Normaliza as probabilidades
        vector<double>prob_normalizadas;
        if(soma_total > 0){
            for(double p : prob_selecao){
                prob_normalizadas.push_back(p / soma_total);
            }
        } else { //Caso todas as probabilidades sejam iguais à 0, ou não haja soma valida (Todos os contadores iguais à 0) //Distribui a probabilidade igualmente entre todos os alphas
            prob_normalizadas.assign(alphas_possiveis.size(), 1.0 / alphas_possiveis.size());
        }

        //Seleção de um alpha com base nas probabilidades
        discrete_distribution<> dist(prob_normalizadas.begin(), prob_normalizadas.end());
        double alpha_selecionado = alphas_possiveis[dist(rng)];

        //Executa o código Guloso Randomizado com o alpha selecionado
        set<char> solucao_gr = encontraMWDS_GulosoRandomizado (grafo, alpha_selecionado);

        //Calculo do peso total da solução encontrada
        int peso_solucao = calculaPesoConjunto(solucao_gr, grafo);

        //Atualização do desempenho de cada alpha
        soma_pesos_alpha[alpha_selecionado] += peso_solucao;
        contador_alpha[alpha_selecionado]++;

        //Mantem-se a melhor solução encontrada até o momento
        if(peso_solucao < menor_peso_melhor_solucao){
            menor_peso_melhor_solucao = peso_solucao;
            melhor_solucao_geral = solucao_gr;
        }
    }
    return melhor_solucao_geral;
 }