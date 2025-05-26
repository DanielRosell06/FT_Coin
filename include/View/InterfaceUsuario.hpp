#pragma once
#include "MenuBase.hpp"
#include "MenuCarteira.hpp"
#include "MenuMovimentacao.hpp"
#include "MenuRelatorios.hpp"
#include "../DAO/InMemoryCarteiraDAO.hpp"
#include "../DAO/InMemoryMovimentacaoDAO.hpp"
#include "../DAO/InMemoryOracleDAO.hpp"

class InterfaceUsuario : public MenuBase {
private:
    InMemoryCarteiraDAO& carteiraDAO;
    InMemoryMovimentacaoDAO& movimentacaoDAO;
    InMemoryOracleDAO& oracleDAO;
    
    MenuCarteira menuCarteira;
    MenuMovimentacao menuMovimentacao;
    MenuRelatorios menuRelatorios;

    void exibirMenuPrincipal();
    void exibirAjuda();
    void exibirCreditos();

public:
    InterfaceUsuario(InMemoryCarteiraDAO& cDao,
                    InMemoryMovimentacaoDAO& mDao,
                    InMemoryOracleDAO& oDao);
    void iniciar();
};