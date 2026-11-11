#pragma once

#include <string>
#include <vector>

// Separa uma linha de texto em campos, usando o delimitador informado.
// Exemplo: separar("ana;32;Recife", ';') devolve {"ana", "32", "Recife"}.
// Uma linha vazia não tem campos.
std::vector<std::string> separar(const std::string& linha, char delimitador);
