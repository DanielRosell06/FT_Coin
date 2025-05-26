#include "../../include/View/MenuRelatorios.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>

MenuRelatorios::MenuRelatorios(
    InMemoryCarteiraDAO& cDao,
    InMemoryMovimentacaoDAO& mDao,
    InMemoryOracleDAO& oDao
) : cartDAO(cDao), movDAO(mDao), oraDAO(oDao) {}

void MenuRelatorios::exibirOpcoes() const {
    std::cout << "1. Listar Carteiras por ID\n"
              << "2. Listar Carteiras por Nome\n"
              << "3. Saldo de uma Carteira\n"
              << "4. Histórico de Movimentações\n"
              << "5. Ganhos/Perdas Totais\n"
              << "6. Voltar\n"
              << "Escolha: ";
}

void MenuRelatorios::listarPorId() const {
    try {
        exibirCabecalho("CARTEIRAS (ORDEM DE ID)");
        auto lista = cartDAO.listarTodos();
        
        if(lista.empty()) {
            std::cout << "Nenhuma carteira cadastrada!\n";
            return;
        }

        for(const auto& c : lista) {
            std::cout << "ID: " << std::setw(4) << c.getId()
                     << " | Titular: " << std::left << std::setw(20) << c.getNomeTitular()
                     << " | Corretora: " << c.getCorretora() << "\n";
        }
    } catch(const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::listarPorNome() const {
    try {
        exibirCabecalho("CARTEIRAS (ORDEM ALFABÉTICA)");
        auto lista = cartDAO.listarTodos();
        
        if(lista.empty()) {
            std::cout << "Nenhuma carteira cadastrada!\n";
            return;
        }

        std::sort(lista.begin(), lista.end(),
            [](const Carteira& a, const Carteira& b) {
                return a.getNomeTitular() < b.getNomeTitular();
            });

        for(const auto& c : lista) {
            std::cout << "Titular: " << std::left << std::setw(20) << c.getNomeTitular()
                     << " | ID: " << std::setw(4) << c.getId()
                     << " | Corretora: " << c.getCorretora() << "\n";
        }
    } catch(const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirSaldo() const {
    try {
        exibirCabecalho("SALDO DA CARTEIRA");
        int id;
        std::cout << "ID da Carteira: ";
        
        if(!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inválido!");
        }
        limparBuffer();

        auto carteira = cartDAO.buscarPorId(id);
        if(!carteira) {
            throw std::runtime_error("Carteira não encontrada!");
        }

        auto movimentacoes = movDAO.listarPorCarteira(id);
        double saldo = 0.0;
        for(const auto& m : movimentacoes) {
            saldo += (m.getTipoOperacao() == 'C') ? m.getQuantidade() : -m.getQuantidade();
        }

        // Obter data atual formatada
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        char dataAtual[11];
        std::strftime(dataAtual, sizeof(dataAtual), "%Y-%m-%d", now);

        double cotacao = oraDAO.buscarCotacaoPorData(dataAtual);
        if(cotacao < 0) {
            throw std::runtime_error("Cotação não disponível para a data atual!");
        }

        std::cout << "\nSaldo em Quantidade: " << std::fixed << std::setprecision(4) << saldo << " FT\n"
                 << "Valor Atual (" << dataAtual << "): R$ "
                 << std::fixed << std::setprecision(2) << (saldo * cotacao) << "\n";

    } catch(const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirHistorico() const {
    try {
        exibirCabecalho("HISTÓRICO DE MOVIMENTAÇÕES");
        int id;
        std::cout << "ID da Carteira: ";
        
        if(!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inválido!");
        }
        limparBuffer();

        auto carteira = cartDAO.buscarPorId(id);
        if(!carteira) {
            throw std::runtime_error("Carteira não encontrada!");
        }

        auto movimentacoes = movDAO.listarPorCarteira(id);
        if(movimentacoes.empty()) {
            std::cout << "Nenhuma movimentação registrada!\n";
            return;
        }

        std::cout << "\n" << std::left
                 << std::setw(12) << "Data"
                 << std::setw(8) << "Tipo"
                 << std::setw(12) << "Quantidade"
                 << "Valor (R$)\n"
                 << std::string(45, '-') << "\n";

        for(const auto& m : movimentacoes) {
            double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
            std::cout << std::setw(12) << m.getDataOperacao()
                     << std::setw(8) << (m.getTipoOperacao() == 'C' ? "Compra" : "Venda")
                     << std::setw(12) << std::fixed << std::setprecision(4) << m.getQuantidade()
                     << "R$ " << std::fixed << std::setprecision(2) 
                     << (m.getQuantidade() * cotacao) << "\n";
        }

    } catch(const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirGanhosPerdas() const {
    try {
        exibirCabecalho("GANHOS/PERDAS TOTAIS");
        auto carteiras = cartDAO.listarTodos();
        auto cotacoes = oraDAO.listarTodasCotacoes();

        if(cotacoes.empty()) {
            throw std::runtime_error("Nenhuma cotação registrada!");
        }

        double ultimaCotacao = cotacoes.back().getCotacao();

        for(const auto& c : carteiras) {
            auto movimentacoes = movDAO.listarPorCarteira(c.getId());
            if(movimentacoes.empty()) continue;

            double totalInvestido = 0.0;
            double saldoQuantidade = 0.0;

            for(const auto& m : movimentacoes) {
                double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
                if(m.getTipoOperacao() == 'C') {
                    totalInvestido += m.getQuantidade() * cotacao;
                    saldoQuantidade += m.getQuantidade();
                } else {
                    totalInvestido -= m.getQuantidade() * cotacao;
                    saldoQuantidade -= m.getQuantidade();
                }
            }

            double valorAtual = saldoQuantidade * ultimaCotacao;
            double resultado = valorAtual - totalInvestido;

            std::cout << "\nCarteira ID: " << c.getId()
                     << " | Titular: " << c.getNomeTitular()
                     << "\nInvestimento Total: R$ " << std::fixed << std::setprecision(2) << totalInvestido
                     << "\nValor Atual: R$ " << valorAtual
                     << "\nResultado: R$ " << resultado 
                     << " (" << (resultado >= 0 ? "Lucro" : "Prejuízo") << ")"
                     << "\n" << std::string(40, '-') << "\n";
        }

    } catch(const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibir() {
    int opcao;
    do {
        exibirCabecalho("RELATÓRIOS");
        exibirOpcoes();

        if(!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "\nOpção inválida!\n";
            aguardarEnter();
            continue;
        }
        limparBuffer();

        switch(opcao) {
            case 1: listarPorId(); break;
            case 2: listarPorNome(); break;
            case 3: exibirSaldo(); break;
            case 4: exibirHistorico(); break;
            case 5: exibirGanhosPerdas(); break;
            case 6: break;
            default:
                std::cout << "\nOpção inválida!\n";
                aguardarEnter();
        }
    } while(opcao != 6);
}