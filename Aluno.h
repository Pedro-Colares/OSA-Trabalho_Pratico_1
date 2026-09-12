#ifndef ALUNO_H
#define ALUNO_H

#include <string>

class Aluno {
private:
    int matricula;
    std::string nome;
    int idade;
    std::string curso;
    std::string cidade;
    std::string uf;
    float cra;

public:
    static constexpr int TAMANHO_REGISTRO_FIXO = 120;

    Aluno();
    Aluno(int matricula, const std::string& nome, int idade,
          const std::string& curso, const std::string& cidade,
          const std::string& uf, float cra);

    int getMatricula() const;
    void setMatricula(int matricula);

    const std::string& getNome() const;
    void setNome(const std::string& nome);

    int getIdade() const;
    void setIdade(int idade);

    const std::string& getCurso() const;
    void setCurso(const std::string& curso);

    const std::string& getCidade() const;
    void setCidade(const std::string& cidade);

    const std::string& getUf() const;
    void setUf(const std::string& uf);

    float getCra() const;
    void setCra(float cra);

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
