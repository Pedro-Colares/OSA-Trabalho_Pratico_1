#include "Benchmark.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
    Benchmark benchmark;

    ofstream resultados("resultados.csv");

    resultados << "N,TempoDireto,TempoSequencial\n";

    resultados.close();

    ofstream metricas("metricas.csv");

    metricas << "Formato,N,TamanhoDisco,BytesUteis,Eficiencia,TempoLeitura\n";

    metricas.close();

    benchmark.executar("arquivos/alunos_1k.csv");
    benchmark.executar("arquivos/alunos_10k.csv");
    benchmark.executar("arquivos/alunos_50k.csv");
    benchmark.executar("arquivos/alunos_100k.csv");

    return 0;
}