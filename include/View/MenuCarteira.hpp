#pragma once
#include "MenuBase.hpp"
#include "../DAO/InMemoryCarteiraDAO.hpp"
#include "../Model/Carteira.hpp"

class MenuCarteira : public MenuBase {
private:
    InMemoryCarteiraDAO& dao;
    
    void exibirOpcoes() const;
    void criarCarteira();
    void editarCarteira();
    void excluirCarteira();
    void listarCarteiras() const;

public:
    explicit MenuCarteira(InMemoryCarteiraDAO& dao);
    void exibir();
};