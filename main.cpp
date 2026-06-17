#include <iostream>

#include "AnalisadorLexico.h"
#include "ErroLexico.h"

using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Erro: informe o arquivo de entrada\n";
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo.txt>\n";
        return 1;
    }

    try
    {
        AnalisadorLexico scanner(argv[1]);
        do
        {
            scanner.s0();

            switch (scanner.tokenAtual)
            {
            case NUM:
                cout << "<NUM> ";
                break;
            case VAR:
                cout << "<VAR> ";
                break;
            case OP:
                cout << "<OP> ";
                break;
            case ABRE_PAR:
                cout << "<ABRE_PAR> ";
                break;
            case FECHA_PAR:
                cout << "<FECHA_PAR> ";
                break;
            case ABRE_COL:
                cout << "<ABRE_COL> ";
                break;
            case FECHA_COL:
                cout << "<FECHA_COL> ";
                break;
            case ABRE_CHA:
                cout << "<ABRE_CHA> ";
                break;
            case FECHA_CHA:
                cout << "<FECHA_CHA> ";
                break;
            case T_EOF:
                cout << "<EOF>\n";
                break;
            default:
                break;
            }
        } while (scanner.tokenAtual != T_EOF);

        cout << "Analise lexica concluida com sucesso!\n";
    }
    catch (const ErroLexico &e)
    {
        cerr << "\n"
             << e.what() << "\nFalha na analise lexica.\n";
        return 1;
    }
    catch (const exception &e)
    {
        cerr << "\nErro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}