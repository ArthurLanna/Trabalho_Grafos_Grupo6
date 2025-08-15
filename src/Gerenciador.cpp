#include "Gerenciador.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <time.h> //Calcular o tempo de execução
#include "Testes.h"

// definição da constante da pasta de saída
const string Gerenciador::pastaSaida = "Output";

// implementação da função para criar a pasta
void Gerenciador::criaPastaSaida(){
    if (!filesystem::exists(pastaSaida)) { 
        try {
            filesystem::create_directory(pastaSaida); //
            cout << "Pasta '" << pastaSaida << "' criada com sucesso." << endl;
        } catch (const filesystem::filesystem_error& e) {
            cerr << "Erro ao criar a pasta '" << pastaSaida << "': " << e.what() << endl;
        }
    } else {
        cout << "Pasta '" << pastaSaida << "' ja existe." << endl;
    }
}

void Gerenciador::comandos(Grafo* grafo) {

    //algoritimo para garantir que a pasta de saida exista
    static bool pastaSaidaCriada = false;
    if(!pastaSaidaCriada){
        criaPastaSaida();
        pastaSaidaCriada = true;
    }

    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algorítmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algorítmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(i) Minimum-Weighted Dominating Set (Algorítmo Guloso);"<<endl;
    cout<<"(j) Minimum-Weighted Dominating Set (Algorítmo Guloso Randomizado);"<<endl;
    cout<<"(k) Minimum-Weighted Dominating Set (Algorítmo Guloso Randomizado Reativo);"<<endl;
    cout<<"(l) 10 execuções de cada algoritmo para a instância apresentada;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    //algoritimo para garantir a abertura dos arquivos de saida, e para garantir que os arquivos sempre estarão vazios no começo da execução do código
    static bool arquivo_limpo = false;
    if (!arquivo_limpo) {
        FILE* fecho_trans_dir = fopen((pastaSaida + "/fecho_trans_dir.txt").c_str(), "w");
        FILE* fecho_trans_indir = fopen((pastaSaida + "/fecho_trans_indir.txt").c_str(), "w"); 
        FILE* Dijkstra = fopen((pastaSaida + "/caminho_minimo_dijkstra.txt").c_str(), "w");
        FILE* Floyd = fopen((pastaSaida + "/caminho_minimo_floyd.txt").c_str(), "w");
        FILE* AgmPrim = fopen((pastaSaida + "/agm_prim.txt").c_str(), "w");
        FILE* AgmKruskal = fopen((pastaSaida + "/agm_kruskal.txt").c_str(), "w");
        FILE* ArvoreProfundidade = fopen((pastaSaida + "/arvore_caminhamento_profundidade.txt").c_str(),"w");
        FILE* RaioDiametroCentroPeriferia = fopen((pastaSaida + "/raio_diametro_centro_periferia.txt").c_str(),"w");
        FILE* MwdsGuloso = fopen((pastaSaida + "/mwds_guloso.txt").c_str(), "w");
        FILE* MwdsGulosoRandomizado = fopen((pastaSaida + "/mwds_guloso_randomizado.txt").c_str(), "w");
        FILE* MwdsGulosoRandomizadoReativo = fopen((pastaSaida + "/mwds_guloso_randomizado_reativo.txt").c_str(), "w");
        FILE* testes_a = fopen((pastaSaida + "/resultados.txt").c_str(), "w");
        if (fecho_trans_dir != NULL) {
            fclose(fecho_trans_dir);
        }
        if (fecho_trans_indir != NULL) {
            fclose(fecho_trans_indir);
        }
        if (Dijkstra != NULL) {
            fclose(Dijkstra);
        }
        if (Floyd != NULL){
            fclose(Floyd);
        }
        if (AgmPrim != NULL){
            fclose(AgmPrim);
        }
        if (AgmKruskal != NULL){
            fclose(AgmKruskal);
        }
        if (ArvoreProfundidade != NULL){
            fclose(ArvoreProfundidade);
        }
        if (RaioDiametroCentroPeriferia != NULL){
            fclose(RaioDiametroCentroPeriferia);
        }
        if(MwdsGuloso != NULL){
            fclose(MwdsGuloso);
        }
        if(MwdsGulosoRandomizado != NULL){
            fclose(MwdsGulosoRandomizado);
        }
        if(MwdsGulosoRandomizadoReativo != NULL){
            fclose(MwdsGulosoRandomizadoReativo);
        }
        if(testes_a != NULL){
            fclose(testes_a);
        }
    arquivo_limpo = true;
}


    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {
            if(!grafo->verificaDirecionado()){
                cout << "Grafo precisa ser direcionado!" << endl << endl;
                break;
            }

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            for(int i = 0; i < fecho_transitivo_direto.size(); i++){
                cout << fecho_transitivo_direto[i] << " ";
            }
            cout << endl;
            
            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                FILE *fecho_trans_dir = fopen((pastaSaida + "/fecho_trans_dir.txt").c_str(), "a");
                if (fecho_trans_dir == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                }
                else{
                    //fprintf(fecho_trans_dir,"fecho transitivo direto de %c: ",id_no);
                    for(int i = 0; i < fecho_transitivo_direto.size(); i++){
                        fprintf(fecho_trans_dir,"%c",fecho_transitivo_direto[i]);
                        if(i != fecho_transitivo_direto.size()-1){
                            fprintf(fecho_trans_dir,",");
                        }
                    }
                    fprintf(fecho_trans_dir,"\n");
                    
                }
                fclose(fecho_trans_dir);
            }


            break;
        }

        case 'b':{
            if(!grafo->verificaDirecionado()){
                cout << "Grafo precisa ser direcionado!" << endl << endl;
                break;
            }

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            for(int i = 0; i < fecho_transitivo_indireto.size(); i++){
                cout << fecho_transitivo_indireto[i] << " ";
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                FILE *fecho_trans_indir = fopen((pastaSaida + "/fecho_trans_indir.txt").c_str(), "a");
                if (fecho_trans_indir == NULL){
                    printf("Erro ao abrir o arquivo.\n");
                }
                else {
                    //fprintf(fecho_trans_indir, "fecho transitivo indireto de %c: ", id_no);
                    for(int i = 0; i < fecho_transitivo_indireto.size(); i++){
                        fprintf(fecho_trans_indir, "%c", fecho_transitivo_indireto[i]);
                        if(i != fecho_transitivo_indireto.size()-1){
                            fprintf(fecho_trans_indir, ",");
                        }
                    }
                    fprintf(fecho_trans_indir, "\n");
                }
                fclose(fecho_trans_indir);
            }

            break;
        }

        case 'c': {
            if(!grafo->verificaPonderadoAresta()){
                cout << "Grafo nao e ponderado na aresta" << endl << endl;
                break;
            }
            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            for(int i = 0; i < caminho_minimo_dijkstra.size(); i++){
                cout << caminho_minimo_dijkstra[i];
                if(i != caminho_minimo_dijkstra.size()-1){
                    cout << ",";
                }
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                FILE *Dijkstra = fopen((pastaSaida + "/caminho_minimo_dijkstra.txt").c_str(), "a");
                if (Dijkstra == NULL){
                    printf("Erro ao abrir o arquivo.\n");
                }
                else{
                    //fprintf(Dijkstra, "caminho minimo dijkstra do no %c para o no %c: ", id_no_1,id_no_2);
                    for(int i = 0; i < caminho_minimo_dijkstra.size(); i++){
                        fprintf(Dijkstra,"%c",caminho_minimo_dijkstra[i]);
                        if(i != caminho_minimo_dijkstra.size()-1){
                            fprintf(Dijkstra,",");
                        }
                    }
                    fprintf(Dijkstra,"\n");
                }
                fclose(Dijkstra);
            }


            break;
        }

        case 'd': {
            if(!grafo->verificaPonderadoAresta()){
                cout << "Grafo nao e ponderado na aresta" << endl << endl;
                break;
            }
            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            for(int i = 0; i < caminho_minimo_floyd.size(); i++){
                cout << caminho_minimo_floyd[i];
                if (i != caminho_minimo_floyd.size()-1){

                    cout << ",";
                }
            }
            cout << endl;
            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                FILE *Floyd = fopen((pastaSaida + "/caminho_minimo_floyd.txt").c_str(), "a");
                if(Floyd == NULL){
                    printf("Erro ao abrir o arquivo. \n");
                }
                else{
                    for (int i = 0; i < caminho_minimo_floyd.size(); i++){
                        fprintf(Floyd, "%c", caminho_minimo_floyd[i]);
                        if (i != caminho_minimo_floyd.size()-1){
                            fprintf(Floyd, ",");
                        }
                    }
                    fprintf(Floyd, "\n");
                }
                fclose(Floyd);
            }

            break;
        }

        case 'e': {

            if (!(grafo->verificaPonderadoAresta())){
                cout << "Grafo precisa ser ponderado na aresta!" << endl;
                break;
            }

            int tam;
            cout<<"Digite o tamanho do subconjunto (0 para selecionar o grafo inteiro): ";
            cin>>tam;

            if(tam >= 0 && tam <= grafo->getOrdem()) {

                if(tam == 0){
                    tam = grafo->getOrdem();
                }

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                arvore_geradora_minima_prim->imprimeListaAdjacencia();

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                   FILE *AgmPrim = fopen((pastaSaida + "/agm_prim.txt").c_str(), "a");
                   if(AgmPrim == NULL){
                     printf("Erro ao abrir o arquivo. \n");
                   } else{
                        arvore_geradora_minima_prim->imprimeListaAdjacenciaEmArquivo(AgmPrim);
                   }
                   fclose(AgmPrim);
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            if (!(grafo->verificaPonderadoAresta())){
                cout << "Grafo precisa ser ponderado na aresta!" << endl;
                break;
            }

            int tam;
            cout<<"Digite o tamanho do subconjunto (0 para selecionar o grafo inteiro): ";
            cin>>tam;

            if(tam >= 0 && tam <= grafo->getOrdem()) {

                if(tam == 0){
                    tam = grafo->getOrdem();
                }

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                arvore_geradora_minima_kruskal->imprimeListaAdjacencia();

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    FILE *AgmKruskal = fopen((pastaSaida + "/agm_kruskal.txt").c_str(), "a");
                    if(AgmKruskal == NULL){
                        printf("Erro ao abrir o arquivo. \n");
                    } else{
                        arvore_geradora_minima_kruskal->imprimeListaAdjacenciaEmArquivo(AgmKruskal);
                    }
                   fclose(AgmKruskal);
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            arvore_caminhamento_profundidade->imprimeListaAdjacencia();

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                FILE* ArvoreProfundidade = fopen((pastaSaida + "/arvore_caminhamento_profundidade.txt").c_str(),"a");
                if(ArvoreProfundidade == NULL){
                    printf("Erro ao abrir o arquivo. \n");
                }else{
                    arvore_caminhamento_profundidade->imprimeListaAdjacenciaEmArquivo(ArvoreProfundidade);
                }
                fclose(ArvoreProfundidade);
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {

            int raio = grafo->raio();
            int diametro = grafo->diametro();
            vector<char> centro = grafo->centro();
            vector<char> periferia = grafo->periferia();

            cout << endl << raio << endl;
            cout << diametro << endl;
            for (int i = 0; i < centro.size(); i++){
                cout << centro[i];
                if (i != centro.size()-1) cout << ",";
            }
            cout << endl;
            for (int i = 0; i < periferia.size(); i++){
                cout << periferia[i];
                if (i != periferia.size()-1) cout << ",";
            }
            cout << endl << endl;

            if(pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt")) {
                FILE* RaioDiametroCentroPeriferia = fopen((pastaSaida + "/raio_diametro_centro_periferia.txt").c_str(), "a");
                if(RaioDiametroCentroPeriferia == NULL){
                    printf("Erro ao abrir o arquivo. \n");
                }else{
                    fprintf(RaioDiametroCentroPeriferia, "%d \n", raio);
                    fprintf(RaioDiametroCentroPeriferia, "%d \n", diametro);
                    for (int i = 0; i < centro.size(); i++){
                        fprintf(RaioDiametroCentroPeriferia, "%c", centro[i]);
                        if(i != centro.size()-1) fprintf(RaioDiametroCentroPeriferia, ",");
                    }
                    fprintf(RaioDiametroCentroPeriferia, "\n");
                    for (int i = 0; i < periferia.size(); i++){
                        fprintf(RaioDiametroCentroPeriferia, "%c", periferia[i]);
                        if(i != periferia.size()-1) fprintf(RaioDiametroCentroPeriferia, ",");
                    }
                    fprintf(RaioDiametroCentroPeriferia, "\n");
                }
                fclose(RaioDiametroCentroPeriferia);
            }
            break;
        }

        case 'i': {
            if (!grafo->verificaPonderadoVertice()) {

                cout << "Grafo não ponderado nos vértices, o algoritmo rodará assumindo todos os pesos como 1." << endl;
            }
            double start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
            set<char> mwds_guloso = Guloso::encontraMWDS_Guloso(grafo);
            double stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
            int peso_conjunto = Guloso::calculaPesoConjunto(mwds_guloso, grafo);

            cout << "Minimum-Weighted Dominating Set (Guloso): { ";
            bool primeiro = true;
            for (char id : mwds_guloso) {
                if (!primeiro) {
                    cout << ", ";
                }
                cout << id;
                primeiro = false;
            }
            cout << " }" << endl << "Peso do Conjunto : " << peso_conjunto << endl;
            cout << "Tempo de execucao: " << stop << endl;

            if (pergunta_imprimir_arquivo("mwds_guloso.txt")) {
                FILE *MwdsGuloso = fopen((pastaSaida + "/mwds_guloso.txt").c_str(), "a");
                if (MwdsGuloso == NULL) {
                    printf("Erro ao abrir o arquivo para MWDS Guloso.\n");
                } else {
                    fprintf(MwdsGuloso, "MWDS (Guloso): { ");
                    primeiro = true;
                    for (char id : mwds_guloso) {
                        if (!primeiro) {
                            fprintf(MwdsGuloso, ", ");
                        }
                        fprintf(MwdsGuloso, "%c", id);
                        primeiro = false;
                    }
                    fprintf(MwdsGuloso, " }\n");
                    fprintf(MwdsGuloso, "Peso do conjunto : ");
                    fprintf(MwdsGuloso, "%d\n", peso_conjunto);
                    fprintf(MwdsGuloso, "Tempo de execucao: %lf ",stop);
                }
                fclose(MwdsGuloso);
            }
            break;
        }

        case 'j':{
            if(!grafo->verificaPonderadoVertice()){
                cout << "Grafo não ponderado nos vértices, o algoritmo rodará assumindo todos os pesos como 1." << endl;
            }
            double alpha;
            cout << "Selecione a abordagem desejada para o parametro alfa:" << endl;
            cout << "(1) Alfa predefinido em codigo (0,5);" << endl;
            cout << "(2) Alfa informado pelo usuario;" << endl;
            cout << "(3) Alfa gerado aleatóriamente com valores entre 0 e 1;" << endl;
            char casoAlpha;
            cin >> casoAlpha;
            switch (casoAlpha){
                case '1': {
                    alpha = 0.5;
                    break;
                }
                case '2': {
                    cout << "Informe o valor desejado para alfa entre 0 e 1: " << endl;
                    do{
                        cin >> alpha;
                        if ((alpha < 0) || (alpha > 1)){
                            cout << "Valor invalido para Alfa, digite novamente um valor entre 0 e 1: " << endl;
                        }
                    } while ((alpha < 0) || (alpha > 1));

                    break;
                }
                case '3': {
                    alpha = geraNumeros();
                    cout << "O Alfa será '" << alpha << "' nessa execução." << endl;
                    break;
                }
                default: {
                    cout<<"Opção inválida"<<endl;
                }
            }
            double start = 0;
            start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
            set<char> mwds_guloso_randomizado = Guloso::encontraMWDS_GulosoRandomizado(grafo,alpha);
            double stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
            int peso_conjunto = Guloso::calculaPesoConjunto(mwds_guloso_randomizado, grafo);

            cout << "Minimum-Weighted Dominating Set (Guloso Randomizado): { ";
            bool primeiro = true;
            for (char id : mwds_guloso_randomizado) {
                if (!primeiro) {
                    cout << ", ";
                }
                cout << id;
                primeiro = false;
            }
            cout << " }" << endl << "Peso do Conjunto : " << peso_conjunto << endl;
            cout << "Tempo de execucao: " << stop << endl;

            if(pergunta_imprimir_arquivo("mwds_guloso_randomizado.txt")){
                FILE* MwdsGulosoRandomizado = fopen((pastaSaida + "/mwds_guloso_randomizado.txt").c_str(), "a");
                if(MwdsGulosoRandomizado == NULL){
                    printf("Erro ao abrir o arquivo para MWDS Guloso Randomizado.\n");
                } else {
                    fprintf(MwdsGulosoRandomizado, "MWDS (Guloso Randomizado): { ");
                    primeiro = true;
                    for (char id : mwds_guloso_randomizado) {
                        if (!primeiro) {
                            fprintf(MwdsGulosoRandomizado, ", ");
                        }
                        fprintf(MwdsGulosoRandomizado, "%c", id);
                        primeiro = false;
                    }
                    fprintf(MwdsGulosoRandomizado, " }\n");
                    fprintf(MwdsGulosoRandomizado, "Peso do conjunto : ");
                    fprintf(MwdsGulosoRandomizado, "%d\n", peso_conjunto);
                    fprintf(MwdsGulosoRandomizado, "Tempo de execucao: %lf ",stop);
                }
                fclose(MwdsGulosoRandomizado);
            }
            break;
        }
        
        case 'k': {
            if(!grafo->verificaPonderadoVertice()){
                cout << "Grafo não ponderado nos vértices, o algoritmo rodará assumindo todos os pesos como 1." << endl;
            }
            int num_alphas;
            double alpha;
            vector<double> alphas_possiveis;
            int num_iteracao_alpha;
            int num_blocos_aprendizagem;
            cout << "Informe o número de alfas que devem ser testados:" << endl;
            cin >> num_alphas;
            for(int i = 0; i < num_alphas; i++){
                cout << "Informe um valor entre 0.0 e 1.0 para o alfa " << i+1 << " :" << endl;
                cin >> alpha;
                if((alpha < 0) || (alpha > 1)){
                    do {
                        cout << "Valor invalido para alfa " << i+1 << " digite novamente : " << endl;
                        cin >> alpha;

                    } while((alpha < 0) || (alpha > 1));
                }
                alphas_possiveis.push_back(alpha);
            }
            cout << "Informe o numero de vezes que cada alfa será testado antes de ser reavaliado :" << endl;
            cin >> num_iteracao_alpha;
            cout << "Informe o número de blocos de aprendizagem desejado :" << endl;
            cin >> num_blocos_aprendizagem;

            double start = 0;
            start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
            set<char> mwds_guloso_randomizado_reativo = Guloso::encontraMWDS_GulosoRandomizadoReativo(grafo, alphas_possiveis, num_iteracao_alpha, num_blocos_aprendizagem);
            double stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
            int peso_conjunto = Guloso::calculaPesoConjunto(mwds_guloso_randomizado_reativo, grafo);

            cout << "Minimum-Weighted Dominating Set (Guloso Randomizado Reativo): { ";
            bool primeiro = true;
            for(char id : mwds_guloso_randomizado_reativo){
                if(!primeiro){
                    cout << ", ";
                }
                cout << id;
                primeiro = false;
            }
            cout << " }" << endl << "Peso do conjunto : " << peso_conjunto << endl;
            cout << "Tempo de execucao: " << stop << endl;

            if(pergunta_imprimir_arquivo("mwds_guloso_randomizado_reativo.txt")){
                FILE* MwdsGulosoRandomizadoReativo = fopen((pastaSaida + "/mwds_guloso_randomizado_reativo.txt").c_str(), "a");
                if(MwdsGulosoRandomizadoReativo == NULL){
                    cout << "Erro ao abrir o arquivo para MWDS Guloso Randomizado Reativo." << endl;
                } else {
                    fprintf(MwdsGulosoRandomizadoReativo, "MWDS (Guloso Randomizado Reativo) : { ");
                    bool primeiro = true;
                    for(char c : mwds_guloso_randomizado_reativo){
                        if(!primeiro){
                            fprintf(MwdsGulosoRandomizadoReativo, ", ");
                        }
                        fprintf(MwdsGulosoRandomizadoReativo, "%c", c);
                        primeiro = false;
                    }
                    fprintf(MwdsGulosoRandomizadoReativo, " }\n");
                    fprintf(MwdsGulosoRandomizadoReativo, "Peso do conjunto : %d\n", peso_conjunto);
                    fprintf(MwdsGulosoRandomizadoReativo, "Tempo de execucao: %lf ",stop);
                }
                fclose(MwdsGulosoRandomizadoReativo);
            }
            break;
        }

        case 'l': {
            Testes testes;

            //Testes do Algoritmo Guloso
            double start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
            set<char> melhor_Guloso = testes.testa_Guloso(grafo);
            double stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
            int peso_conjunto_Guloso = Guloso::calculaPesoConjunto(melhor_Guloso, grafo);

            cout << "Melhor Conjunto Guloso: { ";
            bool primeiro = true;
            for (char id : melhor_Guloso) {
                if (!primeiro) {
                    cout << ", ";
                }
                cout << id;
                primeiro = false;
            }
            cout << " }" << endl << "Peso do Conjunto : " << peso_conjunto_Guloso << endl;
            cout << "Tempo de execucao: " << stop << endl;

            if (pergunta_imprimir_arquivo("resultados.txt")) {
                FILE *testes_a = fopen((pastaSaida + "/resultados.txt").c_str(), "a");
                if (testes_a == NULL) {
                    printf("Erro ao abrir o arquivo para Impressão dos resultados dos testes.\n");
                } else {
                    fprintf(testes_a, "MWDS (Guloso): { ");
                    primeiro = true;
                    for (char id : melhor_Guloso) {
                        if (!primeiro) {
                            fprintf(testes_a, ", ");
                        }
                        fprintf(testes_a, "%c", id);
                        primeiro = false;
                    }
                    fprintf(testes_a, " }\n");
                    fprintf(testes_a, "Peso do conjunto : ");
                    fprintf(testes_a, "%d\n", peso_conjunto_Guloso);
                    fprintf(testes_a, "Tempo de execucao: %lf ",stop);
                    fprintf(testes_a, "\n\n");
                }
                fclose(testes_a);
            }

            //Testes do Algoritmo Guloso Randomizado
            //Teste com o alfa = 0.15
            teste_gulosoRandomizado(grafo, 0.15);
            //Teste com o alfa = 0.5
            teste_gulosoRandomizado(grafo, 0.5);
            //Teste com o alfa = 0.72
            teste_gulosoRandomizado(grafo, 0.72);

            //Testes do Algoritmo Guloso Randomizado Reativo
            vector<double> alphas_possiveis;
            alphas_possiveis.push_back(0.15);
            alphas_possiveis.push_back(0.5);
            alphas_possiveis.push_back(0.72);
            start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
            set<char> melhor_Reativo = testes.testa_GulosoReativo(grafo, alphas_possiveis, 10, 10);
            stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
            int peso_conjunto_Reativo = Guloso::calculaPesoConjunto(melhor_Reativo, grafo);

            cout << "Melhor resposta Guloso Reativo: { ";
            primeiro = true;
            for(char id : melhor_Reativo){
                if(!primeiro){
                    cout << ", ";
                }
                cout << id;
                primeiro = false;
            }
            cout << " }" << endl << "Peso do conjunto : " << peso_conjunto_Reativo << endl;
            cout << "Tempo de execucao: " << stop << endl;

            if(pergunta_imprimir_arquivo("resultados.txt")){
                FILE* testes_a = fopen((pastaSaida + "/resultados.txt").c_str(), "a");
                if(testes_a == NULL){
                    cout << "Erro ao abrir o arquivo para MWDS Guloso Randomizado Reativo." << endl;
                } else {
                    fprintf(testes_a, "MWDS (Guloso Randomizado Reativo) : { ");
                    bool primeiro = true;
                    for(char c : melhor_Reativo){
                        if(!primeiro){
                            fprintf(testes_a, ", ");
                        }
                        fprintf(testes_a, "%c", c);
                        primeiro = false;
                    }
                    fprintf(testes_a, " }\n");
                    fprintf(testes_a, "Peso do conjunto : %d\n", peso_conjunto_Reativo);
                    fprintf(testes_a, "Tempo de execucao: %lf ",stop);
                    fprintf(testes_a, "\n \n");
                }
                fclose(testes_a);
            }

            break;
        }

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};

    //algoritimo para coletar todos os ids sem interface com usuário
    if(tam == grafo->getOrdem()){
        for(No* no : grafo->getListaAdj()){
            ids.push_back(no->getId());
        }
        return ids;
    }

    //algoritimo para coletar um subconjunto especifico de ids com interface com usuário
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->getListaAdj()){
            if(no->getId() == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<< pastaSaida << "/" <<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

double Gerenciador::geraNumeros(){
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> dist(0.0 , 1.0);
    double aux = dist(rng);
    aux = floor(aux * 100.0)/100.0;
    return aux;
}


void Gerenciador::teste_gulosoRandomizado(Grafo* grafo, double alpha){
    Testes testes;
    double start = 0;
    start = 100.0f*clock()/CLOCKS_PER_SEC; //Inicia a contagem do tempo de execução
    set<char> melhor_Randomizado = testes.testa_GulosoRandomizado(grafo, alpha);
    double stop = (100.0f*clock()/CLOCKS_PER_SEC-start)/100.0f;//Para a contagem do tempo de execução
    int peso_conjunto_Randomizado = Guloso::calculaPesoConjunto(melhor_Randomizado, grafo);

    cout<< "Alpha = "<< alpha <<endl;
    cout << "Melhor Resposta Guloso Randomizado: { ";
    bool primeiro = true;
    for (char id : melhor_Randomizado) {
        if (!primeiro) {
            cout << ", ";
        }
        cout << id;
        primeiro = false;
    }
    cout << " }" << endl << "Peso do Conjunto : " << peso_conjunto_Randomizado << endl;
    cout << "Tempo de execucao: " << stop << endl;

    if(pergunta_imprimir_arquivo("resultados.txt")){
        FILE* testes_a = fopen((pastaSaida + "/resultados.txt").c_str(), "a");
        if(testes_a == NULL){
            printf("Erro ao abrir o arquivo para Testes.\n");
        } else {
            fprintf(testes_a, "MWDS (Guloso Randomizado): { ");
            primeiro = true;
            for (char id : melhor_Randomizado) {
                if (!primeiro) {
                    fprintf(testes_a, ", ");
                }
                fprintf(testes_a, "%c", id);
                primeiro = false;
            }
            fprintf(testes_a, " }\n");
            fprintf(testes_a, "Alfa : %.2lf\n", alpha);
            fprintf(testes_a, "Peso do conjunto : ");
            fprintf(testes_a, "%d\n", peso_conjunto_Randomizado);
            fprintf(testes_a, "Tempo de execucao: %lf ",stop);
            fprintf(testes_a, "\n\n");
        }
        fclose(testes_a);
    }
}