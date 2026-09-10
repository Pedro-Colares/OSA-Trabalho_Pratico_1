#include "Aluno.h"
#include <iostream>
#include <cstring>

using namespace std;

int Aluno::packFixo(char *buffer){
    int posicao = 0;
    
    memcpy(buffer+posicao, &matricula, sizeof(matricula));
    posicao+=sizeof(matricula);

    memcpy(buffer+posicao, &nome, sizeof(nome));
    posicao+=sizeof(nome);

    memcpy(buffer+posicao, &idade, sizeof(idade));
    posicao+=sizeof(idade);
    
    memcpy(buffer+posicao, &curso, sizeof(curso));
    posicao+=sizeof(curso);

    memcpy(buffer+posicao, &cidade, sizeof(cidade));
    posicao+=sizeof(cidade);

    memcpy(buffer+posicao, &uf, sizeof(uf));
    posicao+=sizeof(uf);

    memcpy(buffer+posicao, &cra, sizeof(cra));
    posicao+=sizeof(cra);

    return posicao;
}

string Aluno::packDelimitado(){
    return to_string(matricula) + "#" + nome + "#" + to_string(idade) + "#" + curso +
           "#" + cidade + "#" + uf + "#" + to_string(cra); 
}

int Aluno::packIndicador(char *buffer){
    int posicao = 0;

    int tam = sizeof(matricula);
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, &matricula, tam);
    posicao+=tam;

    tam = nome.size();
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, nome.data(), tam);
    posicao+=tam;

    tam = sizeof(idade);
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, &idade, tam);
    posicao+=tam;
    
    tam = curso.size();
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, curso.data(), tam);
    posicao+=tam;

    tam = cidade.size();
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, cidade.data(), tam);
    posicao+=tam;

    tam = uf.size();
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, uf.data(), tam);
    posicao+=tam;

    tam = sizeof(cra);
    memcpy(buffer+posicao, &tam, sizeof(int));
    posicao+=sizeof(int);
    memcpy(buffer+posicao, &cra, tam);
    posicao+=tam;

    return posicao;
}

void Aluno::unpackFixo(const char *buffer){
    int posicao = 0;

    memcpy(&matricula, buffer+posicao, sizeof(matricula));
    posicao+=sizeof(matricula);

    nome.assign(buffer+posicao, 50);
    posicao+=50;

    memcpy(&idade, buffer+posicao, sizeof(idade));
    posicao+=sizeof(idade);
    
    curso.assign(buffer+posicao, 50);
    posicao+=50;

    cidade.assign(buffer+posicao, 50);
    posicao+=50;

    uf.assign(buffer+posicao, sizeof(uf));
    posicao+=sizeof(uf);

    memcpy(&cra, buffer+posicao ,sizeof(cra));
    posicao+=sizeof(cra);

}
void Aluno::unpackDelimitado(const string &buffer){
    int posicao = 0, fim;

    fim = buffer.find('#', posicao);
    matricula = stoi(buffer.substr(posicao, fim-posicao));
    posicao = fim+1;

    fim = buffer.find('#', posicao);
    nome = buffer.substr(posicao, fim-posicao);
    posicao = fim+1;

    fim = buffer.find('#', posicao);
    idade = stoi(buffer.substr(posicao, fim-posicao));
    posicao = fim+1;

    fim = buffer.find('#', posicao);
    curso = buffer.substr(posicao, fim-posicao);
    posicao = fim+1;

    fim = buffer.find('#', posicao);
    cidade = buffer.substr(posicao, fim-posicao);
    posicao = fim+1;

    fim = buffer.find('#', posicao);
    uf = buffer.substr(posicao, fim-posicao);
    posicao = fim+1;
    
    cra = stof(buffer.substr(posicao));
}
void Aluno::unpackIndicador(const char *buffer){
    int posicao = 0, tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    memcpy(&matricula, buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    nome.assign(buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    memcpy(&idade, buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    curso.assign(buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    cidade.assign(buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    uf.assign(buffer+posicao, tam);
    posicao+=tam;

    memcpy(&tam, buffer+posicao, sizeof(int));
    posicao+=sizeof(int);
    memcpy(&cra, buffer+posicao, tam);
    posicao+=tam;
}

int Aluno::getBytesUteis(){
    return sizeof(matricula) + nome.size() + sizeof(idade) + curso.size() + cidade.size()
     + uf.size() + sizeof(cra);
}

void Aluno::imprimir() {
    cout << "Matricula: " << matricula << endl;
    cout << "Nome: " << nome << endl;
    cout << "Idade: " << idade << endl;
    cout << "Curso: " << curso << endl;
    cout << "Cidade: " << cidade << endl;
    cout << "UF: " << uf << endl;
    cout << "CRA: " << cra << endl;
}