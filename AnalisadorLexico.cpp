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
        s1();
        return;
    }

    else if (car == '[')
    {
        lecar();
        s2();
    }

    else if (car == '(')
    {
        lecar();
        s3();
    }

    else if (car == '{')
    {
        lecar();
        s4();
    }

    else if (ehnumero(car))
    {
        lecar();
        s11();
    }

    else if (ehletra(car))
    {
        lecar();
        s8();
    }

    else if (car == '+' || car == '-' || car == '*' || car == '/')
    {
        lecar();
        s12();
    }

    else if (car == ']')
    {
        lecar();
        s5();
    }

    else if (car == ')')
    {
        lecar();
        s6();
    }

    else if (car == '}')
    {
        lecar();
        s7();
    }

    else
    {
        throw ErroLexico(car);
    }
}

// Estado de números
void AnalisadorLexico::s11()
{
    tokenAtual = NUM;

    if (ehnumero(car))
    {
        lecar();
        s11();
    }
}

// Estado de variáveis
void AnalisadorLexico::s8()
{
    tokenAtual = VAR;

    if (ehletra(car))
    {
        lecar();
        s8();
    }
    else if (ehnumero(car))
    {
        lecar();
        s10();
    }
    else if (car == '_')
    {
        lecar();
        s9();
    }
}

void AnalisadorLexico::s10()
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
        s8();
    }
    else if (car == '_')
    {
        lecar();
        s9();
    }
}

void AnalisadorLexico::s9()
{
    tokenAtual = VAR;

    if (car == '_')
    {
        lecar();
        s9();
    }
    else if (ehletra(car))
    {
        lecar();
        s8();
    }
    else if (ehnumero(car))
    {
        lecar();
        s10();
    }
}

// Operadores e delimitadores
void AnalisadorLexico::s12()
{
    tokenAtual = OP;
}

void AnalisadorLexico::s3()
{
    tokenAtual = ABRE_PAR;
}

void AnalisadorLexico::s6()
{
    tokenAtual = FECHA_PAR;
}

void AnalisadorLexico::s2()
{
    tokenAtual = ABRE_COL;
}

void AnalisadorLexico::s5()
{
    tokenAtual = FECHA_COL;
}

void AnalisadorLexico::s4()
{
    tokenAtual = ABRE_CHA;
}

void AnalisadorLexico::s7()
{
    tokenAtual = FECHA_CHA;
}

// Estado de fim do arquivo
void AnalisadorLexico::s1()
{
    tokenAtual = T_EOF;
}
