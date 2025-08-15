#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>
#include <functional>
#include <tuple>
#include <string>
#include <filesystem>
#include "Grafo.h"
#include "Aresta.h"
#include "No.h"

using namespace std;

//definição da constante da pasta de entrada
const string Grafo::pastaEntrada = "Input";

//construtor que recebe o arquivo
Grafo::Grafo(int argc,char *argv[]) {
    leArquivo(argc, argv); 
    in_direcionado = flags[0] - '0';
    in_ponderado_vertice = flags[2] - '0';
    in_ponderado_aresta = flags[1] - '0';
    criaListaAdj();

    //DEBUG
    // imprimeListaAdjacencia();
}

//construtor vazio
Grafo::Grafo(){}

//destrutor
Grafo::~Grafo() {
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestasAdj) {
            delete aresta;
        }
        delete no;
    }
    lista_adj.clear();
    nos.clear();
    arestas.clear();
}

//função para verificar se o grafo é direcionado
bool Grafo::verificaDirecionado(){
    return this->in_direcionado;
}

//função para verificar se o grafo é ponderado nas arestas
bool Grafo::verificaPonderadoAresta(){
    return this->in_ponderado_aresta;
}

//função para verificar se o grafo é ponderado nos vertices
bool Grafo::verificaPonderadoVertice(){
    return this->in_ponderado_vertice;
}

//função que retorna a ordem do grafo
int Grafo::getOrdem(){
    return this->ordem;
}

vector<No*> Grafo::getListaAdj(){
    return this->lista_adj;
}

//função para leitura da instância
void Grafo::leArquivo(int argc, char *argv[]){
    int cont = 0;
    int i = 0;
    char c;
    int linha_atual = 0;
    string linha;
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << endl;
        exit(1);
    }

    string caminhoCompleto = pastaEntrada + "/" + argv[1];
    ifstream arquivo(caminhoCompleto);

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        exit(1);
    }
    string primeiraLinha;
    if (getline(arquivo, primeiraLinha)) {
        istringstream iss(primeiraLinha);
        iss >> flags[0] >> flags[1] >> flags[2];
    }

    // --- Segunda linha: ordem dos vértices ---
    if (getline(arquivo, linha)) {
        ordem = stoi(linha);
    }

    // --- Lista de nós ---
    for (i = 0; i < ordem && getline(arquivo, linha); ++i) {
        nos.push_back(linha);
    }

    // --- Lista de arestas ---
    while (getline(arquivo, linha)) {
        arestas.push_back(linha);
    }

    arquivo.close();

    // Debug opcional
    /*
    for (auto &n : nos) cout << "No: " << n << endl;
    for (auto &a : arestas) cout << "Aresta: " << a << endl;
    cout << "Flags: " << flags[0] << " " << flags[1] << " " << flags[2] << endl;
    */
}

//função que cria a lista de adjacência do grafo baseado nos dados lidos da instância
void Grafo::criaListaAdj() {
    char id_na_fonte, id_no_alvo;
    int pesoAresta = 0;

    for (size_t i = 0; i < arestas.size(); ++i) {
        string linha = arestas[i];
        if (linha.empty()) continue;

        // Remove possível '\r' no fim (arquivos Windows)
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();

        // Tokeniza a linha: espera pelo menos "fonte alvo" e opcionalmente "peso"
        istringstream iss(linha);
        string tokenFonte, tokenAlvo, tokenPeso;
        if (!(iss >> tokenFonte >> tokenAlvo)) {
            cerr << "Formato inválido na linha de aresta: \"" << linha << "\"\n";
            continue;
        }

        // usa o primeiro caractere do token como id (presumindo ids de 1 char)
        id_na_fonte = tokenFonte[0];
        id_no_alvo  = tokenAlvo[0];

        // obtém o peso da aresta (se aplicável)
        pesoAresta = 0;
        if (in_ponderado_aresta) {
            if (iss >> tokenPeso) {
                try {
                    pesoAresta = stoi(tokenPeso);
                } catch (const exception &e) {
                    cerr << "Peso inválido na linha de aresta: \"" << linha
                              << "\" — tratando como 0\n";
                    pesoAresta = 0;
                }
            } else {
                // não veio token de peso: considera 0 (ou trate como erro, se preferir)
                pesoAresta = 0;
            }
        }

        int pesoFonte = in_ponderado_vertice ? obterPesoVertice(id_na_fonte) : 0;
        int pesoAlvo  = in_ponderado_vertice ? obterPesoVertice(id_no_alvo)  : 0;

        Aresta* aresta = new Aresta(in_direcionado, in_ponderado_aresta,
                                   id_na_fonte, id_no_alvo, pesoAresta);
        No* no_fonte = buscaOuCriaNo(id_na_fonte, pesoFonte);
        No* no_alvo  = buscaOuCriaNo(id_no_alvo,  pesoAlvo);

        // Debug
        //cout << "No->getID: " << no_fonte->getId()
                  //<< "  No->getPeso: " << no_fonte->getPeso()
                  //<< "  Aresta->getPeso: " << aresta->getPeso() << endl;

        no_fonte->adicionaVizinho(aresta);
        if (!in_direcionado) {
            // cria aresta simétrica (cópia) para grafo não-direcionado
            no_alvo->adicionaVizinho(
                new Aresta(in_direcionado, in_ponderado_aresta,
                           id_no_alvo, id_na_fonte, pesoAresta));
        }
    }
}

//função auxiliar que verifica se um nó já existe antes de cria-lo
No* Grafo::buscaOuCriaNo(char id, int pesoVertice) {
    for (No* no : lista_adj) {
        if (no->getId() == id) {
            return no;
        }
    }
    No* novo_no = new No(in_ponderado_vertice, pesoVertice, id);
    lista_adj.push_back(novo_no);
    return novo_no;
}

//função para imprimir a lista de adjacência
void Grafo::imprimeListaAdjacencia() {
    int i = 0;
    for (No* no : lista_adj) {
        cout << no->getId() << ": ";
        for (Aresta* aresta : no->arestasAdj) {
            cout << aresta->id_no_alvo;
            if(i != no->arestasAdj.size() - 1){
                cout << " -> ";
            }
            i++;
        }
        i = 0;
        cout << endl;
    }
}

//função para imprimir a lista de adjacência do grafo em um arquivo
void Grafo::imprimeListaAdjacenciaEmArquivo(FILE *a){
    int i = 0;
    for (No* no : lista_adj){
        fprintf(a, "%c: ", no->getId());
        for(Aresta* aresta : no->arestasAdj){
            fprintf(a, "%c", aresta->id_no_alvo);
            if(i != no->arestasAdj.size() - 1){
                fprintf(a, " -> ");
            }
            i++;
        }
        i = 0;
        fprintf(a, "\n");
    }
}

//função para obter o peso de um vértice baseado no seu id
int Grafo::obterPesoVertice(char id) {
    for (auto &linha : nos) {
        istringstream iss(linha);
        string tokenId, tokenPeso;
        iss >> tokenId;
        if (tokenId[0] == id) { // se IDs são char
            if (iss >> tokenPeso) {
                return stoi(tokenPeso);
            } else {
                return 0;
            }
        }
    }
    return 0;
}

void Grafo::buscaEmProfundidade(char id_atual, set<char>& visitados){
    visitados.insert(id_atual);
    No* no = buscaNo(id_atual);
    if (no == nullptr) return;
    for (Aresta* a : no->arestasAdj) {
        char vizinho = a->getIdNoAlvo();
        if (visitados.find(vizinho) == visitados.end()) {
            buscaEmProfundidade(vizinho, visitados);
        }
    }
}

void Grafo::buscaEmProfundidadeReversa(char id_atual, set<char>& visitados) {
    visitados.insert(id_atual);
    for (No* no : lista_adj) {
        for (Aresta* a : no->arestasAdj) {
            if (a->getIdNoAlvo() == id_atual) {
                char id_fonte = no->getId();
                if (visitados.find(id_fonte) == visitados.end()) {
                    buscaEmProfundidadeReversa(id_fonte, visitados);
                }
            }
        }
    }
}


No* Grafo::buscaNo(char id) {
    for (No* no : lista_adj) {
        if (no->getId() == id) {
            return no;
        }
    }
    return nullptr;
}

 void Grafo::bep_para_arvore(char id_atual, set<char>& visitados, Grafo* arvore){
    visitados.insert(id_atual);
    arvore->buscaOuCriaNo(id_atual, this->obterPesoVertice(id_atual));
    No* no_original = this->buscaNo(id_atual);
    if (no_original == nullptr){
        return;
    }
    for (Aresta* aresta : no_original->arestasAdj) {
        char vizinho = aresta->getIdNoAlvo();
        if (visitados.find(vizinho) == visitados.end()) {
            No* no_pai_arvore = arvore->buscaNo(id_atual);
            No* no_filho_arvore = arvore->buscaOuCriaNo(vizinho, this->obterPesoVertice(vizinho));
            int peso_aresta = aresta->getPeso(); 
            no_pai_arvore->adicionaVizinho(new Aresta(false, arvore->in_ponderado_aresta, id_atual, vizinho, peso_aresta));
            no_filho_arvore->adicionaVizinho(new Aresta(false, arvore->in_ponderado_aresta, vizinho, id_atual, peso_aresta));
            bep_para_arvore(vizinho, visitados, arvore);
        }
    }
 }

 void Grafo::calcularMetricasDeDistancia() {
    if (this->metricas_calculadas) {
        return;
    }
    if (!this->verificaPonderadoAresta()) {
        cout << "Erro: Raio, diametro, etc. requerem um grafo ponderado nas arestas." << endl;
        return;
    }

    const int INF = 1e9;
    int n = this->lista_adj.size();

    map<char, int> indice;
    vector<char> ids;
    for (int i = 0; i < n; i++) {
        char id = this->lista_adj[i]->getId();
        indice[id] = i;
        ids.push_back(id);
    }
    vector<vector<int>> dist(n, vector<int>(n, INF));

    for (int i = 0; i < n; i++) dist[i][i] = 0;

    for (No* no : this->lista_adj) {
        int u = indice[no->getId()];
        for (Aresta* a : no->arestasAdj) {
            if (indice.count(a->getIdNoAlvo())) {
                int v = indice[a->getIdNoAlvo()];
                dist[u][v] = min(dist[u][v], a->getPeso());
            }
        }
    }
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    this->m_excentricidades.clear();
    for (int i = 0; i < n; i++) {
        int max_dist = 0;
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                max_dist = INF;
                break;
            }
            if (dist[i][j] > max_dist) {
                max_dist = dist[i][j];
            }
        }
        this->m_excentricidades[ids[i]] = max_dist;
    }
    this->m_raio = INF;
    this->m_diametro = 0;
    for (auto const& [id, ecc] : this->m_excentricidades) {
        if (ecc < this->m_raio) {
            this->m_raio = ecc;
        }
        if (ecc > this->m_diametro && ecc != INF) {
            this->m_diametro = ecc;
        }
    }
    this->m_centro.clear();
    this->m_periferia.clear();
    for (auto const& [id, ecc] : this->m_excentricidades) {
        if (ecc == this->m_raio) {
            this->m_centro.push_back(id);
        }
        if (ecc == this->m_diametro) {
            this->m_periferia.push_back(id);
        }
    }
    this->metricas_calculadas = true;
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    set<char> visitados;
    No *no = NULL;
    vector<char> fecho;
    no = buscaNo(id_no);
    if (!no) {
        cout << "No não encontrado" << endl;
        return {};
    }
    buscaEmProfundidade(id_no, visitados);
    for (char id : visitados) {
        if (id != id_no)
            fecho.push_back(id);
    }
    return fecho;
}


vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    set<char> visitados;
    No *no = NULL;
    vector<char> fecho;
    no = buscaNo(id_no);
    if (!no) {
        cout << "No não encontrado" << endl;
        return {};
    }
    buscaEmProfundidadeReversa(id_no, visitados);
    for (char id : visitados) {
        if (id != id_no)
            fecho.push_back(id);
    }
    return fecho;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    const int INF = 1e9;
    int n = nos.size();
    vector<char> ids;                
    for (const string& nome_no : nos) {
        if (!nome_no.empty())
            ids.push_back(nome_no[0]);
    }
    vector<int> dist(n, INF);              
    vector<bool> visitado(n, false);       
    vector<int> anterior(n, -1);           
    
    int origem = -1;
    for (int i = 0; i < n; i++) {
        if (ids[i] == id_no_a) {
            origem = i;
            break;
        }
    }

    if (origem == -1) {
        cout << "Nó de origem não encontrado.\n";
        return {};
    }

    dist[origem] = 0;

    
    for (int count = 0; count < n; count++) {
        int u = -1;

        for (int i = 0; i < n; i++) {
            if (!visitado[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }

        if (u == -1 || dist[u] == INF) break;

        visitado[u] = true;

        No* atual = buscaNo(ids[u]);
        for (int k = 0; k < atual->arestasAdj.size(); k++) {
            Aresta* a = atual->arestasAdj[k];
            char vid = a->id_no_alvo;

            int v = -1;
            for (int i = 0; i < n; i++) {
                if (ids[i] == vid) {
                    v = i;
                    break;
                }
            }

            if (v == -1) continue;

            if (dist[u] + a->peso < dist[v]) {
                dist[v] = dist[u] + a->peso;
                anterior[v] = u;
            }
        }
    }

    int destino = -1;
    for (int i = 0; i < n; i++) {
        if (ids[i] == id_no_b) {
            destino = i;
            break;
        }
    }

    if (destino == -1 || dist[destino] == INF) {
        cout << "Caminho não encontrado.\n";
        return {};
    }

    vector<char> caminho_temp;
    for (int at = destino; at != -1; at = anterior[at])
        caminho_temp.push_back(ids[at]);

    vector<char> caminho;
    for (int i = caminho_temp.size() - 1; i >= 0; i--)
        caminho.push_back(caminho_temp[i]);

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    const int INF = 1e9;
    int n = lista_adj.size();

    // Mapear cada id de nó para um índice
    map<char, int> indice;
    vector<char> ids;
    for (int i = 0; i < n; i++) {
        char id = lista_adj[i]->getId();
        indice[id] = i;
        ids.push_back(id);
    }

    // Verificação se os nós existem
    if (indice.find(id_no) == indice.end() || indice.find(id_no_b) == indice.end()) {
        cout << "Nó(s) não encontrado(s)." << endl;
        return {};
    }

    // Inicialização
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> pred(n, vector<int>(n, -1));

    for (int i = 0; i < n; i++) dist[i][i] = 0;

    for (No* no : lista_adj) {
        int u = indice[no->getId()];
        for (Aresta* a : no->arestasAdj) {
            int v = indice[a->getIdNoAlvo()];
            if (a->getPeso() < dist[u][v]) {
                dist[u][v] = a->getPeso();
                pred[u][v] = u;
            }
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }

    // Reconstrução do caminho
    int origem = indice[id_no];
    int destino = indice[id_no_b];
    vector<char> caminho;

    if (dist[origem][destino] == INF || pred[origem][destino] == -1) {
        cout << "Não existe caminho entre os nós." << endl;
        return {};
    }

    // Caminho reconstruído de trás pra frente
    int atual = destino;
    while (atual != origem) {
        caminho.push_back(ids[atual]);
        atual = pred[origem][atual];
        if (atual == -1) {
            cout << "Erro na reconstrução do caminho." << endl;
            return {};
        }
    }
    caminho.push_back(ids[origem]);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}


Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    // O algoritmo de Prim requer um grafo ponderado nas arestas.
    if (!this->verificaPonderadoAresta()) {
        cout << "Erro: O algoritmo de Prim so funciona para grafos com arestas ponderadas." << endl;
        return nullptr;
    }

    // Se nenhum nó for especificado, use todos os nós do grafo.
    if (ids_nos.empty()) {
        for (No* no : this->lista_adj) {
            ids_nos.push_back(no->getId());
        }
    }
    
    if (ids_nos.empty()) {
        cout << "Nenhum no no grafo para calcular a AGM." << endl;
        return nullptr;
    }

    // Conjunto para busca rápida dos nós que pertencem ao subgrafo.
    set<char> nos_validos(ids_nos.begin(), ids_nos.end());

    const int INF = 1e9; // Valor para representar infinito.

    // Fila de prioridade para armazenar pares {peso, id_no}. O 'greater' a torna uma min-priority-queue.
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

    map<char, int> custo;      // custo[v] = custo mínimo para conectar v à AGM
    map<char, char> pai;       // pai[v] = pai de v na AGM
    set<char> na_agm;          // Nós que já estão na AGM

    // Inicialização
    for (char id : ids_nos) {
        custo[id] = INF;
        pai[id] = '\0'; // Caractere nulo para indicar ausência de pai
    }

    // Começar pelo primeiro nó do subconjunto
    char no_inicial = ids_nos[0];
    custo[no_inicial] = 0;
    pq.push({0, no_inicial});

    while (!pq.empty()) {
        char u = pq.top().second;
        pq.pop();

        // Se o nó já foi incluído, pule para evitar ciclos e trabalho redundante.
        if (na_agm.count(u)) {
            continue;
        }

        na_agm.insert(u);
        
        No* no_u = this->buscaNo(u);
        if (no_u == nullptr) continue; // Nó não existe no grafo original

        // Itera sobre todos os vizinhos de 'u'
        for (Aresta* aresta : no_u->arestasAdj) {
            char v = aresta->getIdNoAlvo();

            // Considera o vizinho 'v' apenas se ele pertencer ao subconjunto desejado e ainda não estiver na AGM.
            if (nos_validos.count(v) && na_agm.find(v) == na_agm.end()) {
                int peso = aresta->getPeso();
                if (peso < custo[v]) {
                    custo[v] = peso;
                    pai[v] = u;
                    pq.push({custo[v], v});
                }
            }
        }
    }

    // --- Construção do novo grafo que representa a AGM ---

    // Cria um novo objeto Grafo (assumindo que existe um construtor padrão ou que podemos inicializá-lo manualmente)
    Grafo* agm = new Grafo();
    agm->in_direcionado = false; // Uma AGM é sempre não direcionada
    agm->in_ponderado_aresta = this->in_ponderado_aresta;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;
    agm->ordem = na_agm.size();

    // Adiciona os nós e as arestas da AGM ao novo grafo.
    for (char id_no : ids_nos) {
        // Inclui apenas os nós que se conectaram (componente conexa)
        if (pai[id_no] != '\0' || id_no == no_inicial) {
             agm->nos.push_back(this->buscaNo(id_no) ? to_string(id_no) : ""); // Adiciona à lista de nomes
        }
    }


    for (auto const& [filho, genitor] : pai) {
        if (genitor != '\0') { // Se o nó tem um pai (ou seja, não é a raiz)
            // Obter o peso da aresta original
            int peso_aresta = 0;
            No* no_genitor_original = this->buscaNo(genitor);
            if(no_genitor_original) {
                for(Aresta* a : no_genitor_original->arestasAdj) {
                    if(a->getIdNoAlvo() == filho) {
                        peso_aresta = a->getPeso();
                        break;
                    }
                }
            }
            
            // Obter pesos dos vértices, se houver
            int peso_vertice_genitor = this->in_ponderado_vertice ? this->obterPesoVertice(genitor) : 0;
            int peso_vertice_filho = this->in_ponderado_vertice ? this->obterPesoVertice(filho) : 0;

            // Cria ou busca os nós no novo grafo da AGM
            No* no_genitor = agm->buscaOuCriaNo(genitor, peso_vertice_genitor);
            No* no_filho = agm->buscaOuCriaNo(filho, peso_vertice_filho);

            // Adiciona a aresta nos dois sentidos, pois a AGM é não-direcionada
            Aresta* aresta_ida = new Aresta(false, agm->in_ponderado_aresta, genitor, filho, peso_aresta);
            no_genitor->adicionaVizinho(aresta_ida);

            Aresta* aresta_volta = new Aresta(false, agm->in_ponderado_aresta, filho, genitor, peso_aresta);
            no_filho->adicionaVizinho(aresta_volta);
        }
    }
    
    // Caso especial: se o subgrafo era apenas um nó, certifique-se de que ele existe na AGM.
    if(agm->lista_adj.empty() && !ids_nos.empty()){
         int peso_vertice = this->in_ponderado_vertice ? this->obterPesoVertice(no_inicial) : 0;
         agm->buscaOuCriaNo(no_inicial, peso_vertice);
    }
    
    return agm;
}

Grafo* Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    // Kruskal exige um grafo com arestas ponderadas para ordenar as arestas.
    if (!this->verificaPonderadoAresta()) {
        cout << "Erro: O algoritmo de Kruskal so funciona para grafos com arestas ponderadas." << endl;
        return nullptr;
    }

    // Se nenhum nó for especificado, usa todos os nós do grafo.
    if (ids_nos.empty()) {
        for (No* no : this->lista_adj) {
            ids_nos.push_back(no->getId());
        }
    }
    
    if (ids_nos.empty()) {
        cout << "Nenhum no no grafo para calcular a AGM." << endl;
        return nullptr;
    }

    // --- Passo 1: Coletar todas as arestas relevantes ---
    set<char> nos_validos(ids_nos.begin(), ids_nos.end());
    vector<tuple<int, char, char>> arestas;

    for (char id_no : ids_nos) {
        No* no_origem = this->buscaNo(id_no);
        if (no_origem == nullptr) continue;

        for (Aresta* aresta : no_origem->arestasAdj) {
            char no_alvo = aresta->getIdNoAlvo();
            // Adiciona a aresta apenas se ambos os nós estiverem no subconjunto desejado
            // e para evitar duplicatas (ex: A-B e B-A)
            if (nos_validos.count(no_alvo) && id_no < no_alvo) {
                arestas.emplace_back(aresta->getPeso(), id_no, no_alvo);
            }
        }
    }

    // --- Passo 2: Ordenar as arestas pelo peso ---
    sort(arestas.begin(), arestas.end());

    // --- Passo 3: Inicializar Union-Find e o novo grafo ---
    map<char, char> dsu_parent;
    for (char id : ids_nos) {
        dsu_parent[id] = id; // Cada nó é seu próprio pai inicialmente
    }
    
    // Função 'find' com compressão de caminho para otimização
    function<char(char)> find_set = [&](char v) {
        if (v == dsu_parent[v])
            return v;
        return dsu_parent[v] = find_set(dsu_parent[v]);
    };

    // Função 'unite' para unir dois conjuntos
    auto unite_sets = [&](char a, char b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            dsu_parent[b] = a;
        }
    };
    
    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = this->in_ponderado_aresta;
    agm->in_ponderado_vertice = this->in_ponderado_vertice;
    
    // --- Passo 4: Percorrer arestas ordenadas e construir a AGM ---
    for (const auto& aresta_tupla : arestas) {
        int peso = get<0>(aresta_tupla);
        char u = get<1>(aresta_tupla);
        char v = get<2>(aresta_tupla);

        // Se u e v não estiverem no mesmo conjunto, a aresta não forma ciclo
        if (find_set(u) != find_set(v)) {
            // Adiciona a aresta à AGM
            unite_sets(u, v);
            
            // Obter pesos dos vértices (se houver)
            int peso_u = this->in_ponderado_vertice ? this->obterPesoVertice(u) : 0;
            int peso_v = this->in_ponderado_vertice ? this->obterPesoVertice(v) : 0;

            // Adiciona os nós e a aresta ao novo grafo
            No* no_u = agm->buscaOuCriaNo(u, peso_u);
            No* no_v = agm->buscaOuCriaNo(v, peso_v);
            
            no_u->adicionaVizinho(new Aresta(false, agm->in_ponderado_aresta, u, v, peso));
            no_v->adicionaVizinho(new Aresta(false, agm->in_ponderado_aresta, v, u, peso));
        }
    }
    
    // Atualiza a ordem do novo grafo.
    agm->ordem = agm->lista_adj.size();

    return agm;
}

Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    // Verifica se o nó inicial existe no grafo original
    if (this->buscaNo(id_no) == nullptr) {
        cout << "Erro: No inicial '" << id_no << "' nao encontrado." << endl;
        return nullptr;
    }

    // 1. Cria o novo grafo que será a árvore
    Grafo* arvore = new Grafo();
    arvore->in_direcionado = false; // Árvores são inerentemente não direcionadas
    arvore->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore->in_ponderado_vertice = this->in_ponderado_vertice;

    // 2. Cria o set de visitados
    set<char> visitados;

    // 3. Chama a função auxiliar recursiva para construir a árvore, começando pelo nó inicial
    bep_para_arvore(id_no, visitados, arvore);
    
    // Atualiza a ordem da árvore
    arvore->ordem = arvore->lista_adj.size();

    return arvore;
}

int Grafo::raio() {
    calcularMetricasDeDistancia();
    if (this->m_raio == 1e9) {
        cout << "Grafo e desconexo, raio e infinito." << endl;
        return 0;
    }
    return this->m_raio;
}

int Grafo::diametro() {
    calcularMetricasDeDistancia();
    if (this->m_diametro == 0 && this->lista_adj.size() > 1) {
         cout << "Grafo e desconexo, diametro e infinito." << endl;
         return 0;
    }
    return this->m_diametro;
}

vector<char> Grafo::centro() {
    calcularMetricasDeDistancia();
    return this->m_centro;
}

vector<char> Grafo::periferia() {
    calcularMetricasDeDistancia();
    return this->m_periferia;
}
