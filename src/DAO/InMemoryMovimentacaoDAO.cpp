#include "../../include/DAO/InMemoryMovimentacaoDAO.hpp"
#include <algorithm>

void InMemoryMovimentacaoDAO::criar(Movimentacao& mov) {
    mov.setId(proximoId++);
    movimentacoes.push_back(mov);
}

Movimentacao* InMemoryMovimentacaoDAO::buscarPorId(int id) {
    for (auto& m : movimentacoes) {
        if (m.getId() == id) return &m;
    }
    return nullptr;
}

void InMemoryMovimentacaoDAO::atualizar(Movimentacao& mov) {
    for (auto& m : movimentacoes) {
        if (m.getId() == mov.getId()) {
            m = mov;
            break;
        }
    }
}

void InMemoryMovimentacaoDAO::remover(int id) {
    movimentacoes.erase(std::remove_if(movimentacoes.begin(), movimentacoes.end(),
        [id](Movimentacao& m) { return m.getId() == id; }), movimentacoes.end());
}

std::vector<Movimentacao> InMemoryMovimentacaoDAO::listarPorCarteira(int carteiraId) {
    std::vector<Movimentacao> resultado;
    for (const auto& m : movimentacoes) {
        if (m.getCarteiraId() == carteiraId) {
            resultado.push_back(m);
        }
    }
    return resultado;
}