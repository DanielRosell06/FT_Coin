#include "../../include/DAO/InMemoryOracleDAO.hpp"
#include <algorithm>

void InMemoryOracleDAO::adicionarCotacao(const Oracle& oracle) {
    // Remove cotações duplicadas para a mesma data
    cotacoes.erase(std::remove_if(cotacoes.begin(), cotacoes.end(),
        [&oracle](const Oracle& o) { return o.getData() == oracle.getData(); }), 
        cotacoes.end());
    cotacoes.push_back(oracle);
}

double InMemoryOracleDAO::buscarCotacaoPorData(const std::string& data) {
    for (const auto& o : cotacoes) {
        if (o.getData() == data) {
            return o.getCotacao();
        }
    }
    return 0.0; // Retorna 0 se não encontrar (em produção, tratar erro)
}

std::vector<Oracle> InMemoryOracleDAO::listarTodasCotacoes() {
    return cotacoes;
}