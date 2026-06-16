#include <iostream>  //interação com o usuário
#include <string>    //armazenamento de dados em memória
#include <fstream>   //acesso a arquivos externos (entrada de dados).
#include <stdexcept> //gestão de erros

using namespace std;

// Definição dos Tokens
enum Token
{
    NUM,
    VAR,
    OP,
    ABRE_PAR,
    FECHA_PAR,
    ABRE_COL,
    FECHA_COL,
    ABRE_CHA,
    FECHA_CHA,
    T_EOF,
    VAZIO
};

class ErroLexico : public runtime_error
{
public:
    ErroLexico(char encontrado)
        : runtime_error("Erro Lexico: caractere '" + string(1, encontrado) + "' inesperado.") {}
};

class AnalisadorLexico
{
private:
    string entrada;
    int posicao;
    char carac;

    bool ehnumero(char c)
    {
        return (c >= '0' && c <= '9');
    }

    bool ehletra(char c)
    {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    bool ehletra_num_under(char c)
    {
        return (ehletra(c) || ehnumero(c) || c == '_');
    }

public:
    Token tokenAtual;

    AnalisadorLexico(const string &nomearquivo)
    {
        ifstream arquivo(nomearquivo);
        if (!arquivo.is_open())
        {
            throw runtime_error("Erro ao abrir o arquivo de entrada: " + nomearquivo);
        }

        string linha;
        entrada = "";
        while (getline(arquivo, linha))
        { // le o arquivo linha por linha
            entrada += linha + "\n";
        }
        arquivo.close();

        posicao = 0;
        tokenAtual = VAZIO;
        lecarac();
    }

    // le o prox caracter do buffer,se fim, retorna eof; avança o ponteiro de leitura para +1 posicão
    void lecarac()
    {
        if (posicao < entrada.length())
        {
            carac = entrada[posicao];
            posicao++;
        }
        else
            carac = EOF;
    }

    // ESTADOS

    void s0()
    {

        while (carac == ' ' || carac == '\n' || carac == '\t' || carac == '\r')
        {
            lecarac();
        }

        if (carac == EOF)
        {
            s_eof();
            return;
        }

        if (ehnumero(carac))
        {
            lecarac();
            s1();
        }

        else if (ehletra(carac))
        {
            lecarac();
            s2();
        }

        else if (carac == '+' || carac == '-' || carac == '*' || carac == '/')
        {
            lecarac();
            s3_operador();
        }
        else if (carac == '(')
        {
            lecarac();
            s7_apar();
        }
        else if (carac == ')')
        {
            lecarac();
            s8_fpar();
        }
        else if (carac == '[')
        {
            lecarac();
            s9_acol();
        }
        else if (carac == ']')
        {
            lecarac();
            s10_fcol();
        }
        else if (carac == '{')
        {
            lecarac();
            s11_acha();
        }
        else if (carac == '}')
        {
            lecarac();
            s12_fcha();
        }
        else
            throw ErroLexico(carac);
    }

    void s1()
    {
        tokenAtual = NUM;
        if (ehnumero(carac))
        {
            lecarac();
            s1();
        }
    }

    void s2()
    {
        tokenAtual = VAR;
        if (ehletra_num_under(carac))
        {
            lecarac();
            s2();
        }
    }

    void s3_operador() { tokenAtual = OP; }
    void s7_apar() { tokenAtual = ABRE_PAR; }
    void s8_fpar() { tokenAtual = FECHA_PAR; }
    void s9_acol() { tokenAtual = ABRE_COL; }
    void s10_fcol() { tokenAtual = FECHA_COL; }
    void s11_acha() { tokenAtual = ABRE_CHA; }
    void s12_fcha() { tokenAtual = FECHA_CHA; }

    // Estado de fim
    void s_eof() { tokenAtual = T_EOF; }
};

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