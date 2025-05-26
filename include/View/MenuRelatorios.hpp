#pragma once
#include "MenuBase.hpp"
#include "../DAO/InMemoryCarteiraDAO.hpp"
#include "../DAO/InMemoryMovimentacaoDAO.hpp"
#include "../DAO/InMemoryOracleDAO.hpp"
#include "../Model/Carteira.hpp"
#include "../Model/Movimentacao.hpp"
#include "../Model/Oracle.hpp"

class MenuRelatorios : public MenuBase {
private:
    InMemoryCarteiraDAO& cartDAO;
    InMemoryMovimentacaoDAO& movDAO;
    InMemoryOracleDAO& oraDAO;

    void exibirOpcoes() const;
    void listarPorId() const;
    void listarPorNome() const;
    void exibirSaldo() const;
    void exibirHistorico() const;
    void exibirGanhosPerdas() const;

public:
    MenuRelatorios(InMemoryCarteiraDAO& cDao,
                  InMemoryMovimentacaoDAO& mDao,
                  InMemoryOracleDAO& oDao);
    void exibir();
};