#include "GerenciadorArquivo.h"
#include<iostream>
#include<vector>

using namespace std;

int main(){
    string caminhoCSV = "arquivos/alunos_1k.csv";
    string caminhoBinario = "alunos_dat";

    vector<Aluno> alunos = GerenciadorArquivo::lerCSV(caminhoCSV);
    if(alunos.empty()){
        cout << "Nenhum aluno foi carregado.\n" << endl;
        return 1;
    }

    cout << "Alunos carregados: " << alunos.size() << ".\n" << endl;

    GerenciadorArquivo::salvarFixo(caminhoBinario, alunos);
    vector<Aluno> alunosLidos= GerenciadorArquivo::lerFixo(caminhoBinario);
    cout << "Alunos lidos do arquivo: " << alunosLidos.size() << ".\n" << endl;

    if(!alunosLidos.empty()){
        alunosLidos[0].imprimir();
    }

    Aluno aluno;
    int rrn = 2;
    if(GerenciadorArquivo::lerPorRRN(caminhoBinario, rrn, aluno)){
        cout << "Aluno encontrado no RRN " << rrn << ".\n" << endl;
        aluno.imprimir();
    }

    return 0;
}