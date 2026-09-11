#include "Benchmark.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<random>
#include<chrono>
#include<numeric>
#include<iomanip>
#include<algorithm>

using namespace std;

void Benchmark::executar(const string& caminhoCSV){
    cout << "Caminho recebido: " << caminhoCSV << endl;

}

void Benchmark::medirAcessoDiretoVsSequencial(const string& base, int N){
    ofstream arqTemp(base, ios::binary);
    if(!arqTemp){
        cout << "Erro ao abir arquivo de testes." << endl;
        return;
    }

    Metricas met;
    for(int i=0; i<N; i++){
        met.quantidadeRegistros = i;
        arqTemp.write(reinterpret_cast<const char*>(&met), sizeof(Metricas));
    }

    arqTemp.close();

    ifstream arqSeq(base, ios::binary);
    auto inicioSeq = chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++){
        arqSeq.read(reinterpret_cast<char*>(&met), sizeof(Metricas));
    }
    auto fimSeq = chrono::high_resolution_clock::now();

    arqSeq.close();

    vector<int> indicesAleatorios(N);
    iota(indicesAleatorios.begin(), indicesAleatorios.end(), 0);
    
    random_device rd;
    mt19937 g(rd());
    shuffle(indicesAleatorios.begin(), indicesAleatorios.end(), g);

    ifstream arqDir(base, ios::binary);
    auto inicoDir = chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++){
        int pos = 0;
        arqDir.seekg(pos*sizeof(Metricas), ios::beg);
        arqDir.read(reinterpret_cast<char*>(&met), sizeof(Metricas));
    }
    auto fimDir = chrono::high_resolution_clock::now();
    arqDir.close();

    chrono::duration<double, milli> tempoSeq = fimSeq-inicioSeq;
    chrono::duration<double, milli> tempoDir = fimDir-inicoDir;

    cout << "Medicao - N registros: " << endl;
    cout << "Tempo Sequencial: " << tempoSeq.count() << "ms" << endl;
    cout << "Tempo Direto: " << tempoDir.count() << "ms" << endl;
    cout << "Acesso Direto foi " << (tempoDir.count()/tempoSeq.count()) <<
        "x vezes mais lento que o acesso sequencial" << endl;

}

void Benchmark::imprimirTabelaRelatorio(const std::vector<Metricas>& res){
    if(res.empty()){
        cout << "Nenhum registro" << endl;
        return;
    }

}