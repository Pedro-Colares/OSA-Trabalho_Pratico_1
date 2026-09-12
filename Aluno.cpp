#include "Aluno.h"
#include <iostream>
#include <cstring>

using namespace std;

Aluno::Aluno(){
    matricula = 0;
    nome = "";
    idade = 0;
    curso = "";
    cidade = "";
    uf = "";
    cra = 0.0;

}

Aluno::Aluno(int matricula, const string &nome, int idade,
             const string &curso, const string &cidade,
             const string &uf, float cra){
                setMatricula(matricula);
                setNome(nome);
                setIdade(idade);
                setCurso(curso);
                setCidade(cidade);
                setUf(uf);
                setCra(cra);
             }

int Aluno::getMatricula() const{
    return matricula;
}

void Aluno::setMatricula(int matricula){
    this->matricula = matricula;
}

const string &Aluno::getNome() const{
    return nome;
}

void Aluno::setNome(const string &nome){
    this->nome = nome;
}

int Aluno::getIdade() const{
    return idade;
}

void Aluno::setIdade(int idade){
    this->idade = idade;
}

const string &Aluno::getCurso() const{
    return curso;
}

void Aluno::setCurso(const string &curso){
    this->curso = curso;
}

const string &Aluno::getCidade() const{
    return cidade;
}

void Aluno::setCidade(const string &cidade){
    this->cidade = cidade;
}

const string &Aluno::getUf() const{
    return uf;
}

void Aluno::setUf(const string &uf){
    this->uf = uf;
}

float Aluno::getCra() const{
    return cra;
}

void Aluno::setCra(float cra){
    this->cra = cra;
}

int Aluno::packFixo(char *buffer){
    int posicao = 0;

    memset(buffer, 0, TAMANHO_REGISTRO_FIXO);

    memcpy(buffer + posicao, &matricula, sizeof(matricula));
    posicao += sizeof(matricula);

    memcpy(buffer + posicao, nome.c_str(), nome.size() < 40 ? nome.size() : 40);
    posicao += 40;

    memcpy(buffer + posicao, &idade, sizeof(idade));
    posicao += sizeof(idade);

    memcpy(buffer + posicao, curso.c_str(), curso.size() < 35 ? curso.size() : 35);
    posicao += 35;

    memcpy(buffer + posicao, cidade.c_str(), cidade.size() < 30 ? cidade.size() : 30);
    posicao += 30;

    memcpy(buffer + posicao, uf.c_str(), uf.size() < 3 ? uf.size() : 3);
    posicao += 3;

    memcpy(buffer + posicao, &cra, sizeof(cra));
    posicao += sizeof(cra);

    return posicao;
}
string Aluno::packDelimitado(){
    return to_string(matricula) + "#" + nome + "#" + to_string(idade) + "#" + curso +
           "#" + cidade + "#" + uf + "#" + to_string(cra);
}

int Aluno::packIndicador(char *buffer){
    int posicao = 0;

    memcpy(buffer + posicao, &matricula, sizeof(matricula));
    posicao = posicao + sizeof(matricula);

    unsigned char tamNome = (unsigned char) nome.size();
    memcpy(buffer + posicao, &tamNome, sizeof(tamNome));
    posicao = posicao + sizeof(tamNome);

    memcpy(buffer + posicao, nome.data(), tamNome);   
    posicao += tamNome;

    memcpy(buffer + posicao, &idade, sizeof(idade));
    posicao += sizeof(idade);

    unsigned char tamCurso = (unsigned char) curso.size();
    memcpy(buffer + posicao, &tamCurso, sizeof(tamCurso));
    posicao += sizeof(tamCurso);
    memcpy(buffer + posicao, curso.data(), tamCurso);
    posicao += tamCurso;

    unsigned char tamCidade = (unsigned char) cidade.size();
    memcpy(buffer + posicao, &tamCidade, sizeof(tamCidade));
    posicao += sizeof(tamCidade);
    memcpy(buffer + posicao, cidade.data(), tamCidade);
    posicao += tamCidade;

    unsigned char tamUf = (unsigned char) uf.size();
    memcpy(buffer + posicao, &tamUf, sizeof(tamUf));
    posicao += sizeof(tamUf);
    memcpy(buffer + posicao, uf.data(), tamUf);
    posicao += tamUf;

    memcpy(buffer + posicao, &cra, sizeof(cra));
    posicao += sizeof(cra);

    return posicao;
}

void Aluno::unpackFixo(const char *buffer){
    int posicao = 0;

    memcpy(&matricula, buffer + posicao, sizeof(matricula));
    posicao += sizeof(matricula);

    nome.assign(buffer + posicao, 40);
    if (size_t fim = nome.find('\0'); fim != string::npos) nome.resize(fim);
    posicao += 40;

    memcpy(&idade, buffer + posicao, sizeof(idade));
    posicao += sizeof(idade);

    curso.assign(buffer + posicao, 35);
    if (size_t fim = curso.find('\0'); fim != string::npos) curso.resize(fim);
    posicao += 35;

    cidade.assign(buffer + posicao, 30);
    if (size_t fim = cidade.find('\0'); fim != string::npos) cidade.resize(fim);
    posicao += 30;

    uf.assign(buffer + posicao, 3);
    if (size_t fim = uf.find('\0'); fim != string::npos) uf.resize(fim);
    posicao += 3;

    memcpy(&cra, buffer + posicao, sizeof(cra));
    posicao += sizeof(cra);
}

void Aluno::unpackDelimitado(const string &buffer){
    int posicao = 0, fim;

    fim = buffer.find('#', posicao);
    matricula = stoi(buffer.substr(posicao, fim - posicao));
    posicao = fim + 1;

    fim = buffer.find('#', posicao);
    nome = buffer.substr(posicao, fim - posicao);
    posicao = fim + 1;

    fim = buffer.find('#', posicao);
    idade = stoi(buffer.substr(posicao, fim - posicao));
    posicao = fim + 1;

    fim = buffer.find('#', posicao);
    curso = buffer.substr(posicao, fim - posicao);
    posicao = fim + 1;

    fim = buffer.find('#', posicao);
    cidade = buffer.substr(posicao, fim - posicao);
    posicao = fim + 1;

    fim = buffer.find('#', posicao);
    uf = buffer.substr(posicao, fim - posicao);
    posicao = fim + 1;

    cra = stof(buffer.substr(posicao));
}

void Aluno::unpackIndicador(const char *buffer){
    int posicao = 0, tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    memcpy(&matricula, buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    nome.assign(buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    memcpy(&idade, buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    curso.assign(buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    cidade.assign(buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    uf.assign(buffer + posicao, tam);
    posicao += tam;

    memcpy(&tam, buffer + posicao, sizeof(int));
    posicao += sizeof(int);
    memcpy(&cra, buffer + posicao, tam);
    posicao += tam;
}

int Aluno::getBytesUteis(){
    return sizeof(matricula) + nome.size() + sizeof(idade) + 
        curso.size() + cidade.size() + uf.size() + sizeof(cra);
}

void Aluno::imprimir(){
    cout << "Matricula: " << matricula << endl;
    cout << "Nome: " << nome << endl;
    cout << "Idade: " << idade << endl;
    cout << "Curso: " << curso << endl;
    cout << "Cidade: " << cidade << endl;
    cout << "UF: " << uf << endl;
    cout << "CRA: " << cra << endl;
}