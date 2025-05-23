#include "../../include/DAO/InMemoryCarteiraDAO.hpp"
#include <algorithm>

void InMemoryCarteiraDAO::criar(Carteira& carteira) {
    carteira.setId(proximoId++);
    carteiras.push_back(carteira);
}

Carteira* InMemoryCarteiraDAO::buscarPorId(int id) {
    for (auto& c : carteiras) {
        if (c.getId() == id) return &c;
    }
    return nullptr;
}

void InMemoryCarteiraDAO::atualizar(Carteira& carteira) {
    for (auto& c : carteiras) {
        if (c.getId() == carteira.getId()) {
            c = carteira;
            break;
        }
    }
}

void InMemoryCarteiraDAO::remover(int id) {
    carteiras.erase(std::remove_if(carteiras.begin(), carteiras.end(),
        [id](Carteira& c) { return c.getId() == id; }), carteiras.end());
}

std::vector<Carteira> InMemoryCarteiraDAO::listarTodos() {
    return carteiras;
}