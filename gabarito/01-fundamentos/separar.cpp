#include "separar.h"

std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    if (linha.empty())
        return {};

    std::vector<std::string> campos;
    std::string::size_type inicio = 0;

    while (true)
    {
        const auto fim = linha.find(delimitador, inicio);
        if (fim == std::string::npos)
        {
            campos.push_back(linha.substr(inicio));
            return campos;
        }

        campos.push_back(linha.substr(inicio, fim - inicio));
        inicio = fim + 1;
    }
}
