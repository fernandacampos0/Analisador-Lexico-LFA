#ifndef ANALISADORLEXICO_H
#define ANALISADORLEXICO_H

#include <string>
#include "Token.h"

using namespace std;

class AnalisadorLexico
{
private:
    string entrada;
    int posicao;
    char car;

    bool ehnumero(char c);
    bool ehletra(char c);
    bool ehletra_num_under(char c);

public:
    Token tokenAtual;

    AnalisadorLexico(const string &nomearquivo);

    void lecar();

    void s0();
    void s1();
    void s2();
    void s3();
    void s4();

    void s5_operador();
    void s7_apar();
    void s8_fpar();
    void s9_acol();
    void s10_fcol();
    void s11_acha();
    void s12_fcha();

    void s_eof();
};

#endif