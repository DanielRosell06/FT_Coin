#pragma once
#include "MenuBase.hpp"
#include "../DAO/InMemoryMovimentacaoDAO.hpp"
#include "../DAO/InMemoryCarteiraDAO.hpp"
#include "../DAO/InMemoryOracleDAO.hpp"
#include "../Model/Movimentacao.hpp"
#include "../Model/Oracle.hpp"

class MenuMovimentacao : public MenuBase {
private:
    InMemoryMovimentacaoDAO& movDAO;
    InMemoryCarteiraDAO& cartDAO;
    InMemoryOracleDAO& oraDAO;

    void exibirOpcoes() const;
    void registrarOperacao(char tipo);
    
public:
    MenuMovimentacao(InMemoryMovimentacaoDAO& mDao, 
                    InMemoryCarteiraDAO& cDao,
                    InMemoryOracleDAO& oDao);
    void exibir();
};