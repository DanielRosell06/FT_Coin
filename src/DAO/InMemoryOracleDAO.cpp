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
    // Se não encontrar, gera uma cotação aleatória entre 150 e 160 para esta data
    double cotacaoAleatoria = 150.0 + static_cast<double>(rand()) / RAND_MAX * 10.0;
    Oracle novaOracle(data, cotacaoAleatoria);
    cotacoes.push_back(novaOracle);
    return cotacaoAleatoria;
}

std::vector<Oracle> InMemoryOracleDAO::listarTodasCotacoes() {
    return cotacoes;
}