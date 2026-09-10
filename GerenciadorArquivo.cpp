#include "GerenciadorArquivo.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

vector<string> split(const string &linha, char delimitador) {
    vector<string> campos;
    size_t posicao = 0;
    size_t fim;
    
    while(true){
        fim = linha.find(delimitador, posicao);
        if(fim == string::npos){
            campos.push_back(linha.substr(posicao, fim - posicao));
            break;
        }
        campos.push_back(linha.substr(posicao, fim - posicao));
        posicao = fim + 1;
    }
    return campos;
}

vector<Aluno> GerenciadorArquivo::lerCSV(string caminho) {
    vector<Aluno> alunos;


    ifstream arquivo(caminho);

    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return alunos;
    }

    string linha;

    getline(arquivo, linha);

   
    while (getline(arquivo, linha)) {
    
        vector<string> campos = split(linha, ',');

        Aluno a;
        a.matricula = stoi(campos[0]);
        a.nome      = campos[1];
        a.idade     = stoi(campos[2]);
        a.curso     = campos[3];
        a.cidade    = campos[4];
        a.uf        = campos[5];
        a.cra       = stof(campos[6]);

        alunos.push_back(a);
    }

    arquivo.close();

    return alunos;
}

bool GerenciadorArquivo::salvarFixo(string arq, vector<Aluno> alunos) {
    ofstream arquivo(arq, ios::binary | ios::out);

    if(!arquivo.is_open()){
        cout << "Erro ao abrir arquivo." << endl;
        return false;
    }

    char buffer[120];

    for (int i = 0; i < alunos.size(); i++) {
        alunos[i].packFixo(buffer);
        arquivo.write(buffer, 120);
    }

    arquivo.close();
    return true;
}

vector<Aluno> GerenciadorArquivo::lerFixo(string arq) {
    vector<Aluno> alunos;

    ifstream arquivo(arq, ios::binary | ios::in);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo." << endl;
        return alunos;
    }

    char buffer[120];

    while (arquivo.read(buffer, 120)) {
        Aluno a;
        a.unpackFixo(buffer);
        alunos.push_back(a);
    }

    arquivo.close();
    return alunos;
}

bool GerenciadorArquivo::lerPorRRN(string arq, int rrn, Aluno& out) {
    ifstream arquivo(arq, ios::binary | ios::in);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo." << endl;
        return false;
    }

    int offset = rrn * 120;
    arquivo.seekg(offset, ios::beg);


    char buffer[120];


    if (!arquivo.read(buffer, 120)) {
        cout << "Erro ao ler registro RRN " << rrn << endl;
        arquivo.close();
        return false;
    }

    out.unpackFixo(buffer);
    arquivo.close();
    return true;
}

bool GerenciadorArquivo::salvarDelimitado(string arq, vector<Aluno> alunos) {
    ofstream arquivo(arq, ios::binary | ios::out);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo." << endl;
        return false;
    }

    for (int i = 0; i < alunos.size(); i++) {
        string linha = alunos[i].packDelimitado();
        arquivo.write(linha.c_str(), linha.size());
        arquivo.put('|');
    }

    arquivo.close();
    return true;
}

long long GerenciadorArquivo::obterTamanhoArquivo(string arq) {
    ifstream arquivo(arq, ios::binary | ios::ate);

    if (!arquivo.is_open()) {
        return -1;
    }

    long long tamanho = arquivo.tellg();
    arquivo.close();
    return tamanho;
}

vector<Aluno> GerenciadorArquivo::lerDelimitado(string arq) {
    vector<Aluno> alunos;

    ifstream arquivo(arq, ios::binary | ios::in);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo." << endl;
        return alunos;
    }

    string registro;

    while (getline(arquivo, registro, '|')) {
        Aluno a;
        a.unpackDelimitado(registro);
        alunos.push_back(a);
    }

    arquivo.close();
    return alunos;
}