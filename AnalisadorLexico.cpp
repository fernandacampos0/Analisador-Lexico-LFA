#include <iostream>  //interação com o usuário
#include <string>    //armazenamento de dados em memória
#include <fstream>   //acesso a arquivos externos (entrada de dados).
#include <stdexcept> //gestão de erros

// Definição dos Tokens
enum Token
{
    NUM,
    VAR,
    OP_ARIT,
    ABRE_PAR,
    FECHA_PAR,
    ABRE_COL,
    FECHA_COL,
    ABRE_CHA,
    FECHA_CHA,
    T_EOF,
    VAZIO
};

// Classe para tratar erro léxico
class ErroLexico : public std::runtime_error
{
public:
    ErroLexico(char encontrado)
        : std::runtime_error("Erro Lexico: caractere '" + std::string(1, encontrado) + "' inesperado.") {}
};

// Classe Analisador Léxico
class AnalisadorLexico
{
private:
    std::string entrada;
    size_t posicao;
    char proxCaractere;

    // Verifica se o caractere é um dígito de 0 a 9
    bool ehDigito(char c)
    {
        return (c >= '0' && c <= '9');
    }

    // Verifica se o caractere é uma letra maiúscula ou minúscula
    bool ehLetra(char c)
    {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    // Verifica se é uma letra, número ou underscore '_'
    bool ehAlfanumericoOuUnderscore(char c)
    {
        return (ehLetra(c) || ehDigito(c) || c == '_');
    }

public:
    Token token_Reconhecido;

    // Construtor recebe arquivo a ser analisado
    AnalisadorLexico(const std::string &nomeArquivo)
    {
        std::ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open())
        {
            throw std::runtime_error("Erro ao abrir o arquivo de entrada: " + nomeArquivo);
        }

        std::string linha;
        entrada = "";
        while (std::getline(arquivo, linha))
        { // le o arquivo linha por linha
            entrada += linha + "\n";
        }
        arquivo.close();

        posicao = 0;
        token_Reconhecido = VAZIO;
        leProxCaractere();
    }

    // le o prox caracter do buffer,se fim, retorna eof; avança o ponteiro de leitura para +1 posicão
    void leProxCaractere()
    {
        if (posicao < entrada.length())
        {
            proxCaractere = entrada[posicao];
            posicao++;
        }
        else
        {
            proxCaractere = EOF; // Representa o fim do arquivo
        }
    }

    // IMPLEMENTAÇÃO DOS ESTADOS

    // Estado Inicial s0
    void s0()
    {
        // espaços, pulos de linha ou tabulações "resetam" a máquina
        while (proxCaractere == ' ' || proxCaractere == '\n' || proxCaractere == '\t' || proxCaractere == '\r')
        {
            leProxCaractere();
        }

        if (proxCaractere == EOF)
        {
            s_eof();
            return;
        }

        // caso inicie com dígito, vai para estado s1
        if (ehDigito(proxCaractere))
        {
            leProxCaractere();
            s1(); // salta para estado de número
        }
        // caso inicie com letra, vai para estado s2
        else if (ehLetra(proxCaractere))
        {
            leProxCaractere();
            s2();
        }
        // caracteres operadores e delimitadores
        else if (proxCaractere == '+' || proxCaractere == '-' || proxCaractere == '*' || proxCaractere == '/')
        {
            leProxCaractere();
            s3_operador_aritmetico(); // Vai para o estado único
        }
        else if (proxCaractere == '(')
        {
            leProxCaractere();
            s7_apar();
        }
        else if (proxCaractere == ')')
        {
            leProxCaractere();
            s8_fpar();
        }
        else if (proxCaractere == '[')
        {
            leProxCaractere();
            s9_acol();
        }
        else if (proxCaractere == ']')
        {
            leProxCaractere();
            s10_fcol();
        }
        else if (proxCaractere == '{')
        {
            leProxCaractere();
            s11_acha();
        }
        else if (proxCaractere == '}')
        {
            leProxCaractere();
            s12_fcha();
        }
        else
        {
            throw ErroLexico(proxCaractere);
        }
    }

    // Estado s1: números inteiros
    void s1()
    {
        token_Reconhecido = NUM;
        // Enquanto vierem dígitos, a máquina continua no loop do estado s1
        if (ehDigito(proxCaractere))
        {
            leProxCaractere();
            s1();
        }
    }

    // Estado s2: variáveis
    void s2()
    {
        token_Reconhecido = VAR;
        // Permite letras, números e underscore após o primeiro caractere válido
        if (ehAlfanumericoOuUnderscore(proxCaractere))
        {
            leProxCaractere();
            s2(); // Loop no próprio estado s2
        }
    }

    // Estados dos operadores e símbolos
    void s3_operador_aritmetico() { token_Reconhecido = OP_ARIT; }
    void s7_apar() { token_Reconhecido = ABRE_PAR; }
    void s8_fpar() { token_Reconhecido = FECHA_PAR; }
    void s9_acol() { token_Reconhecido = ABRE_COL; }
    void s10_fcol() { token_Reconhecido = FECHA_COL; }
    void s11_acha() { token_Reconhecido = ABRE_CHA; }
    void s12_fcha() { token_Reconhecido = FECHA_CHA; }

    // Estado de fim
    void s_eof() { token_Reconhecido = T_EOF; }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Erro: Esqueceu de passar o nome do arquivo de entrada!\n";
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo.txt>\n";
        return 1;
    }

    try
    {
        AnalisadorLexico scanner(argv[1]);
        do
        {
            scanner.s0();

            // Traduz o enum para texto legível no console de testes
            switch (scanner.token_Reconhecido)
            {
            case NUM:
                std::cout << "<NUM> ";
                break;
            case VAR:
                std::cout << "<VAR> ";
                break;
            case OP_ARIT:
                std::cout << "<OP_ARIT> ";
                break;
            case ABRE_PAR:
                std::cout << "<ABRE_PAR> ";
                break;
            case FECHA_PAR:
                std::cout << "<FECHA_PAR> ";
                break;
            case ABRE_COL:
                std::cout << "<ABRE_COL> ";
                break;
            case FECHA_COL:
                std::cout << "<FECHA_COL> ";
                break;
            case ABRE_CHA:
                std::cout << "<ABRE_CHA> ";
                break;
            case FECHA_CHA:
                std::cout << "<FECHA_CHA> ";
                break;
            case T_EOF:
                std::cout << "<EOF>\n";
                break;
            default:
                break;
            }
        } while (scanner.token_Reconhecido != T_EOF);

        std::cout << "Analise lexica concluida com sucesso!\n";
    }
    catch (const ErroLexico &e)
    {
        std::cerr << "\n"
                  << e.what() << "\nFALHA na analise.\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nErro no programa: " << e.what() << "\n";
        return 1;
    }

    return 0;
}