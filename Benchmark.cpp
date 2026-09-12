#include "Benchmark.h"
#include "GerenciadorArquivo.h"
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
    vector<Aluno> alunos = GerenciadorArquivo::lerCSV(caminhoCSV);

    if(alunos.empty()){
        cout << "Nenhum aluno foi carregado de: "
             << caminhoCSV << endl;
        return;
    }

    int bytesUteis = 0;
    for(const Aluno& aluno : alunos){
        bytesUteis += aluno.getBytesUteis();
    }

    size_t posicaoBarra = caminhoCSV.find_last_of("/\\");
    string nomeArquivo = caminhoCSV.substr(posicaoBarra + 1);

    size_t posicaoPonto = nomeArquivo.find_last_of('.');
    string base = nomeArquivo.substr(0, posicaoPonto);

    string arquivoFixo = base + "_fixos.bin";
    string arquivoDelimitado = base + "_delimitados.bin";
    string arquivoIndicador = base + "_indicados.bin";

    GerenciadorArquivo::salvarFixo(arquivoFixo, alunos);
    long long tamanhoFixo = GerenciadorArquivo::obterTamanhoArquivo(arquivoFixo);

    GerenciadorArquivo::salvarDelimitado(arquivoDelimitado, alunos);
    long long tamanhoDelimitado = GerenciadorArquivo::obterTamanhoArquivo(arquivoDelimitado);

    GerenciadorArquivo::salvarIndicador(arquivoIndicador, alunos);
    long long tamanhoIndicador = GerenciadorArquivo::obterTamanhoArquivo(arquivoIndicador);

    long long overheadFixo = tamanhoFixo - bytesUteis;
    long long overheadDelimitado = tamanhoDelimitado - bytesUteis;
    long long overheadIndicador = tamanhoIndicador - bytesUteis;

    double eficienciaFixo = static_cast<double>(bytesUteis)/ tamanhoFixo * 100;
    double eficienciaDelimitado = static_cast<double>(bytesUteis)/ tamanhoDelimitado * 100;
    double eficienciaIndicador = static_cast<double>(bytesUteis)/ tamanhoIndicador * 100;

    auto inicioFixo = chrono::high_resolution_clock::now();
    vector<Aluno> alunosFixo = GerenciadorArquivo::lerFixo(arquivoFixo);
    auto fimFixo = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tempoFixo = fimFixo - inicioFixo;

    auto inicioDelimitado = chrono::high_resolution_clock::now();
    vector<Aluno> alunosDelimitado = GerenciadorArquivo::lerDelimitado(arquivoDelimitado);
    auto fimDelimitado = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tempoDelimitado = fimDelimitado - inicioDelimitado;

    auto inicioIndicador = chrono::high_resolution_clock::now();
    vector<Aluno> alunosIndicador = GerenciadorArquivo::lerIndicador(arquivoIndicador);
    auto fimIndicador = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tempoIndicador = fimIndicador - inicioIndicador;

    fstream metricas("metricas.csv", ios::app);
    metricas << "Fixo," << alunos.size() << "," << tamanhoFixo << "," << 
    bytesUteis << "," << eficienciaFixo << "," << tempoFixo.count() << "\n";

    metricas << "Delimitado," << alunos.size() << "," << tamanhoDelimitado << "," << 
    bytesUteis << "," << eficienciaDelimitado << "," << tempoDelimitado.count() << "\n";

    metricas << "Indicador," << alunos.size() << "," << tamanhoIndicador << "," << 
    bytesUteis << "," << eficienciaIndicador << "," << tempoIndicador.count() << "\n";

    medirAcessoDiretoVsSequencial(base, alunos.size());

    cout << "Caminho recebido: " << caminhoCSV << endl;
}

void Benchmark::medirAcessoDiretoVsSequencial(const string& base, int N){
    string arquivoFixo = base + "_fixos.bin";
    string arquivoDelimitado = base + "_delimitados.bin";

    vector<int> indicesAleatorios(N);
    iota(indicesAleatorios.begin(), indicesAleatorios.end(), 0);

    random_device rd;
    mt19937 g(rd());
    shuffle(indicesAleatorios.begin(), indicesAleatorios.end(), g);
    ifstream arqDir(arquivoFixo, ios::binary);

    if(!arqDir.is_open()){
        cout << "Erro ao abrir arquivoFixo" << endl;
        return;
    }

    char bufferFixo[Aluno::TAMANHO_REGISTRO_FIXO];
    Aluno aluno;
    auto inicioDir = chrono::high_resolution_clock::now();
    for(int i=0; i<100; i++){
        int rrn = indicesAleatorios[i];
        arqDir.seekg(
            static_cast<long long>(rrn) * Aluno::TAMANHO_REGISTRO_FIXO,
            ios::beg
        );

        if(arqDir.read(bufferFixo, Aluno::TAMANHO_REGISTRO_FIXO)){
            aluno.unpackFixo(bufferFixo);
        }
    }

    auto fimDir = chrono::high_resolution_clock::now();
    arqDir.close();
    ifstream arqSeq(arquivoDelimitado, ios::binary);

    if(!arqSeq.is_open()){
        cout << "Erro ao abrir arquivoDelimitado" << endl;
        return;
    }

    auto inicioSeq = chrono::high_resolution_clock::now();
    string registro;
    for(int i=0; i<100; i++){
        int rrn = indicesAleatorios[i];
        arqSeq.clear();
        arqSeq.seekg(0, ios::beg);

        int atual = 0;
        while(getline(arqSeq, registro, '|')){
            if(atual==rrn){
                Aluno alunoEncontrado;
                alunoEncontrado.unpackDelimitado(registro);
                break;
            }

            atual++;
        }
    }

    auto fimSeq = chrono::high_resolution_clock::now();
    arqSeq.close();
    chrono::duration<double, milli> tempoDir = fimDir - inicioDir;
    chrono::duration<double, milli> tempoSeq = fimSeq - inicioSeq;

    cout << "Medicao - N registros: " << N << endl;
    cout << fixed << setprecision(4);
    cout << "Tempo acesso direto: " << tempoDir.count() << " ms" << endl;
    cout << "Tempo acesso sequencial: " << tempoSeq.count() << " ms" << endl;

    ofstream resultados("resultados.csv", ios::app);

    resultados << N << "," << tempoDir.count() << "," << tempoSeq.count() << "\n";

    resultados.close();

    if(tempoDir.count() > 0){
        cout << "Acesso sequencial / direto: " << tempoSeq.count() / tempoDir.count()
        << "x" << endl;
    }
}

void Benchmark::imprimirTabelaRelatorio(const std::vector<Metricas>& res){
    if(res.empty()){
        cout << "Nenhum registro" << endl;
        return;
    }

}