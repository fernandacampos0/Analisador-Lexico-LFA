#ifndef ERROLEXICO_H
#define ERROLEXICO_H

#include <stdexcept>
#include <string>

using namespace std;

class ErroLexico : public runtime_error
{
public:
    ErroLexico(char c)
        : runtime_error("Caractere invalido: " + string(1, c)) {}
};

#endif