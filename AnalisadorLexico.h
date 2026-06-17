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
    void s1();  // EOF
    void s2();  // <ABRE_COL>
    void s3();  // <ABRE_PAR>
    void s4();  // <ABRE_CHA>
    void s5();  // <FECHA_COL>
    void s6();  // <FECHA_PAR>
    void s7();  // <FECHA_CHA>
    void s8();  // <VAR> (Letra)
    void s9();  // <VAR> (Número)
    void s10(); // <VAR> (Underline)
    void s11(); // <NUM>
    void s12(); // <OP>
};

#endif