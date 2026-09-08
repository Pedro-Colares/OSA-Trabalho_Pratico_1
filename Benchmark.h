#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <string>
#include <vector>

#include "Aluno.h"

struct Metricas {
    std::string formato;
    int quantidadeRegistros = 0;
    long long tamanhoArquivo = 0;
    double tempoAcessoDireto = 0.0;
    double tempoAcessoSequencial = 0.0;
};

class Benchmark {
public:
    static void executar(const std::string& caminhoCSV);
    static void medirAcessoDiretoVsSequencial(const std::string& base, int N);
    static void imprimirTabelaRelatorio(const std::vector<Metricas>& res);
};

#endif
