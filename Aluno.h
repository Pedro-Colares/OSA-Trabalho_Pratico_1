#ifndef ALUNO_H
#define ALUNO_H

#include <string>

class Aluno {
public:
    int matricula;
    std::string nome;
    int idade;
    std::string curso;
    std::string cidade;
    std::string uf;
    float cra;

    int packFixo(char* buffer);
    std::string packDelimitado();
    int packIndicador(char* buffer);

    void unpackFixo(const char* buffer);
    void unpackDelimitado(const std::string& buffer);
    void unpackIndicador(const char* buffer);

    int getBytesUteis();
    void imprimir();
};

#endif
