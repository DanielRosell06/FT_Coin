#include <iostream>
#include <iomanip>
#include "../include/Model/Carteira.hpp"
#include "../include/DAO/InMemoryCarteiraDAO.hpp"
#include "../include/Model/Movimentacao.hpp"
#include "../include/DAO/InMemoryMovimentacaoDAO.hpp"
#include "../include/Model/Oracle.hpp"
#include "../include/DAO/InMemoryOracleDAO.hpp"

using namespace std;

void imprimirCarteira(const Carteira& c) {
    cout << "ID: " << c.getId() 
         << " | Titular: " << c.getNomeTitular() 
         << " | Corretora: " << c.getCorretora() << endl;
}

void imprimirMovimentacao(const Movimentacao& m, double cotacao) {
    string tipo = (m.getTipoOperacao() == 'C') ? "COMPRA" : "VENDA";
    double valorTotal = m.getQuantidade() * cotacao;
    
    cout << left << setw(10) << m.getDataOperacao()
         << setw(8) << tipo
         << setw(8) << m.getQuantidade()
         << "R$ " << fixed << setprecision(2) << valorTotal
         << endl;
}

double calcularSaldo(const vector<Movimentacao>& movs, OracleDAO& oracleDAO) {
    double saldo = 0.0;
    for (const auto& m : movs) {
        double cotacao = oracleDAO.buscarCotacaoPorData(m.getDataOperacao());
        if (cotacao > 0) {
            saldo += (m.getTipoOperacao() == 'C') ? m.getQuantidade() : -m.getQuantidade();
        }
    }
    return saldo;
}

int main() {
    InMemoryCarteiraDAO carteiraDAO;
    InMemoryMovimentacaoDAO movimentacaoDAO;
    InMemoryOracleDAO oracleDAO;

    cout << "=== TESTE COMPLETO DO SISTEMA FT_COIN ===" << endl << endl;

    // ========== TESTE DE CARTEIRAS ==========
    cout << "\n[1] Cadastro de Carteiras:\n";
    Carteira c1("João Silva", "Binance");
    Carteira c2("Maria Souza", "Coinbase");
    Carteira c3("Carlos Oliveira", "FTX");

    carteiraDAO.criar(c1);
    carteiraDAO.criar(c2);
    carteiraDAO.criar(c3);

    // Listar todas carteiras
    vector<Carteira> carteiras = carteiraDAO.listarTodos();
    for (const auto& c : carteiras) {
        imprimirCarteira(c);
    }

    // ========== TESTE DE COTAÇÕES ==========
    cout << "\n[2] Cadastro de Cotações:\n";
    oracleDAO.adicionarCotacao(Oracle("2023-01-01", 150.0));
    oracleDAO.adicionarCotacao(Oracle("2023-02-01", 180.5));
    oracleDAO.adicionarCotacao(Oracle("2023-03-01", 210.75));

    // ========== TESTE DE MOVIMENTAÇÕES ==========
    cout << "\n[3] Registro de Movimentações:\n";
    int idJoao = c1.getId();
    
    Movimentacao m1(idJoao, "2023-01-01", 'C', 2.5);
    Movimentacao m2(idJoao, "2023-02-01", 'C', 1.75);
    Movimentacao m3(idJoao, "2023-03-01", 'V', 1.0);

    movimentacaoDAO.criar(m1);
    movimentacaoDAO.criar(m2);
    movimentacaoDAO.criar(m3);

    // ========== RELATÓRIOS ==========
    cout << "\n[4] Relatório da Carteira " << idJoao << ":\n";
    vector<Movimentacao> movs = movimentacaoDAO.listarPorCarteira(idJoao);
    
    cout << "Data      Tipo    Qtd.   Valor\n";
    cout << "--------------------------------\n";
    for (const auto& m : movs) {
        double cotacao = oracleDAO.buscarCotacaoPorData(m.getDataOperacao());
        imprimirMovimentacao(m, cotacao);
    }

    // ========== SALDO E PERFORMANCE ==========
    cout << "\n[5] Saldo e Performance:\n";
    double saldoQuantidade = calcularSaldo(movs, oracleDAO);
    double saldoAtual = saldoQuantidade * oracleDAO.buscarCotacaoPorData("2023-03-01");

    cout << "Saldo em Quantidade: " << saldoQuantidade << " FT\n";
    cout << "Valor Atual (03/2023): R$ " << saldoAtual << endl;

    // ========== TESTE DE ATUALIZAÇÃO ==========
    cout << "\n[6] Teste de Atualização:\n";
    Carteira* c = carteiraDAO.buscarPorId(idJoao);
    if(c) {
        c->setCorretora("Novo Broker");
        carteiraDAO.atualizar(*c);
        cout << "Carteira atualizada:\n";
        imprimirCarteira(*c);
    }

    // ========== TESTE DE EXCLUSÃO ==========
    cout << "\n[7] Teste de Exclusão:\n";
    carteiraDAO.remover(c2.getId());
    cout << "Carteiras após exclusão:\n";
    for (const auto& c : carteiraDAO.listarTodos()) {
        imprimirCarteira(c);
    }

    // ========== TESTE DE ERROS ==========
    cout << "\n[8] Teste de Cotação Inexistente:\n";
    double cotacaoFake = oracleDAO.buscarCotacaoPorData("2999-01-01");
    cout << "Cotação para data inexistente: " << cotacaoFake << endl;

    return 0;
}