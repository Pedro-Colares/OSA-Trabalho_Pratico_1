#include "GerenciadorArquivo.h"
#include <iostream>
#include <fstream>

using namespace std;

vector<string> split(const string &linha, char delimitador) {
    vector<string> campos;
    size_t posicao = 0;
    size_t fim;
    
    while(true){
        fim = linha.find(delimitador, posicao);
        if(fim == string::npos){
            campos.push_back(linha.substr(posicao));
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

        Aluno a(stoi(campos[0]), campos[1], stoi(campos[2]), campos[3],
               campos[4], campos[5], stof(campos[6]));

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

    char buffer[Aluno::TAMANHO_REGISTRO_FIXO];

    for (size_t i = 0; i < alunos.size(); i++) {
        alunos[i].packFixo(buffer);
        arquivo.write(buffer, Aluno::TAMANHO_REGISTRO_FIXO);
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

    char buffer[Aluno::TAMANHO_REGISTRO_FIXO];

    while (arquivo.read(buffer, Aluno::TAMANHO_REGISTRO_FIXO)) {
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

    int offset = rrn * Aluno::TAMANHO_REGISTRO_FIXO;
    arquivo.seekg(offset, ios::beg);


    char buffer[Aluno::TAMANHO_REGISTRO_FIXO];


    if (!arquivo.read(buffer, Aluno::TAMANHO_REGISTRO_FIXO)) {
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

    for (size_t i = 0; i < alunos.size(); i++) {
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

bool GerenciadorArquivo::salvarIndicador(string arq, vector<Aluno> alunos) {
    ofstream arquivo(arq, ios::binary | ios::out);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir arquivo." << endl;
        return false;
    }
    char buffer[124];

    for(size_t i=0; i<alunos.size(); i++) {

        int tamanho = alunos[i].packIndicador(buffer);

        unsigned short tamanhoRegistro =
            static_cast<unsigned short>(tamanho);

        arquivo.write(
            reinterpret_cast<const char*>(&tamanhoRegistro),
            sizeof(tamanhoRegistro)
        );

        arquivo.write(buffer, tamanho);
    }

    arquivo.close();

    return true;
}

vector<Aluno> GerenciadorArquivo::lerIndicador(string arq) {
    vector<Aluno> alunos;
    ifstream arquivo(arq, ios::binary | ios::in);
    if (!arquivo.is_open()){
        cout << "Erro ao abrir arquivo." << endl;
        return alunos;
    }
    while (true){
        unsigned short tamanhoRegistro;
        if(!arquivo.read(
            reinterpret_cast<char*>(&tamanhoRegistro),
            sizeof(tamanhoRegistro))){
            break;
        }

        char buffer[124];
        if (!arquivo.read(buffer, tamanhoRegistro)){
            cout << "Erro ao ler registro." << endl;
            break;
        }

        Aluno a;
        a.unpackIndicador(buffer);
        alunos.push_back(a);
    }

    arquivo.close();

    return alunos;
}