#include "AnalisadorLexico.h"
#include "ErroLexico.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

bool AnalisadorLexico::ehnumero(char c)
{
    return (c >= '0' && c <= '9');
}

bool AnalisadorLexico::ehletra(char c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

// Construtor da classe
AnalisadorLexico::AnalisadorLexico(const string &nomearquivo)
{
    ifstream arquivo(nomearquivo);

    if (!arquivo.is_open())
    {
        throw runtime_error("Erro ao abrir o arquivo de entrada: " + nomearquivo);
    }

    string linha;
    entrada = "";

    while (getline(arquivo, linha))
    {
        entrada += linha + "\n";
    }

    arquivo.close();

    posicao = 0;
    tokenAtual = VAZIO;

    lecar();
}

void AnalisadorLexico::lecar()
{
    if (posicao < entrada.length())
    {
        car = entrada[posicao];
        posicao++;
    }
    else
    {
        car = EOF;
    }
}

// Estado inicial
void AnalisadorLexico::s0()
{
    while (car == ' ' || car == '\n' ||
           car == '\t' || car == '\r')
    {
        lecar();
    }

    if (car == EOF)
    {
        s_eof();
        return;
    }

    if (ehnumero(car))
    {
        lecar();
        s1();
    }

    else if (ehletra(car))
    {
        lecar();
        s2();
    }

    else if (car == '+' || car == '-' || car == '*' || car == '/')
    {
        lecar();
        s5_operador();
    }

    else if (car == '(')
    {
        lecar();
        s7_apar();
    }

    else if (car == ')')
    {
        lecar();
        s8_fpar();
    }

    else if (car == '[')
    {
        lecar();
        s9_acol();
    }

    else if (car == ']')
    {
        lecar();
        s10_fcol();
    }

    else if (car == '{')
    {
        lecar();
        s11_acha();
    }

    else if (car == '}')
    {
        lecar();
        s12_fcha();
    }

    else
    {
        throw ErroLexico(car);
    }
}

// Estado de números
void AnalisadorLexico::s1()
{
    tokenAtual = NUM;

    if (ehnumero(car))
    {
        lecar();
        s1();
    }
}

// Estado de variáveis
void AnalisadorLexico::s2()
{
    tokenAtual = VAR;

    if (ehletra(car))
    {
        lecar();
        s2();
    }
    else if (ehnumero(car))
    {
        lecar();
        s3();
    }
    else if (car == '_')
    {
        lecar();
        s4();
    }
}

void AnalisadorLexico::s3()
{
    tokenAtual = VAR;

    if (ehnumero(car))
    {
        lecar();
        s3();
    }
    else if (ehletra(car))
    {
        lecar();
        s2();
    }
    else if (car == '_')
    {
        lecar();
        s4();
    }
}

void AnalisadorLexico::s4()
{
    tokenAtual = VAR;

    if (car == '_')
    {
        lecar();
        s4();
    }
    else if (ehletra(car))
    {
        lecar();
        s2();
    }
    else if (ehnumero(car))
    {
        lecar();
        s3();
    }
}

// Estados dos operadores e delimitadores
void AnalisadorLexico::s5_operador()
{
    tokenAtual = OP;
}

void AnalisadorLexico::s7_apar()
{
    tokenAtual = ABRE_PAR;
}

void AnalisadorLexico::s8_fpar()
{
    tokenAtual = FECHA_PAR;
}

void AnalisadorLexico::s9_acol()
{
    tokenAtual = ABRE_COL;
}

void AnalisadorLexico::s10_fcol()
{
    tokenAtual = FECHA_COL;
}

void AnalisadorLexico::s11_acha()
{
    tokenAtual = ABRE_CHA;
}

void AnalisadorLexico::s12_fcha()
{
    tokenAtual = FECHA_CHA;
}

// Estado de fim do arquivo
void AnalisadorLexico::s_eof()
{
    tokenAtual = T_EOF;
}
