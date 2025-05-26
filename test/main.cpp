#include <algorithm> // Para sort()
#include <iostream>
#include <iomanip>
#include <limits>                
#include <ctime>
#include "../include/Model/Carteira.hpp"
#include "../include/DAO/InMemoryCarteiraDAO.hpp"
#include "../include/Model/Movimentacao.hpp"
#include "../include/DAO/InMemoryMovimentacaoDAO.hpp"
#include "../include/Model/Oracle.hpp"
#include "../include/DAO/InMemoryOracleDAO.hpp"

using namespace std;

// Protótipos das funções
void exibirMenuPrincipal();
void menuCarteira(InMemoryCarteiraDAO &);
void menuMovimentacao(InMemoryMovimentacaoDAO &, InMemoryCarteiraDAO &, InMemoryOracleDAO &);
void menuRelatorios(InMemoryCarteiraDAO &, InMemoryMovimentacaoDAO &, InMemoryOracleDAO &);
void exibirAjuda();
void exibirCreditos();

// Variáveis globais para os DAOs
InMemoryCarteiraDAO carteiraDAO;
InMemoryMovimentacaoDAO movimentacaoDAO;
InMemoryOracleDAO oracleDAO;

#include <limits> // Adicione este include no topo do arquivo

void limparBufferEntrada()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void aguardarEnter()
{
    cout << "\nPressione Enter para continuar...";
    cin.get(); // Espera pelo Enter
}

int main()
{
    int opcao;

    do
    {
        exibirMenuPrincipal();
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao)
        {
        case 1:
            menuCarteira(carteiraDAO);
            break;
        case 2:
            menuMovimentacao(movimentacaoDAO, carteiraDAO, oracleDAO);
            break;
        case 3:
            menuRelatorios(carteiraDAO, movimentacaoDAO, oracleDAO);
            break;
        case 4:
            exibirAjuda();
            break;
        case 5:
            exibirCreditos();
            break;
        case 6:
            cout << "\nSaindo do sistema...\n";
            break;
        default:
            cout << "\nOpção inválida!\n";
        }
    } while (opcao != 6);

    return 0;
}

// Implementação das funções do menu
void exibirMenuPrincipal()
{
    system("cls || clear");
    cout << "=== FT COIN - Sistema de Gerenciamento ==="
         << "\n1. Carteira"
         << "\n2. Movimentação"
         << "\n3. Relatórios"
         << "\n4. Ajuda"
         << "\n5. Créditos"
         << "\n6. Sair"
         << "\nEscolha uma opção: ";
}

void menuCarteira(InMemoryCarteiraDAO &dao)
{
    int opcao;
    do
    {
        system("cls || clear");
        cout << "=== MENU CARTEIRA ==="
             << "\n1. Nova Carteira"
             << "\n2. Editar Carteira"
             << "\n3. Excluir Carteira"
             << "\n4. Listar Todas"
             << "\n5. Voltar"
             << "\nEscolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao)
        {
        case 1:
        {
            string nome, corretora;
            cout << "\nNovo Cadastro:\nNome do titular: ";
            getline(cin, nome);
            cout << "Corretora: ";
            getline(cin, corretora);

            Carteira nova(nome, corretora);
            dao.criar(nova);
            cout << "\nCarteira criada com sucesso! ID: " << nova.getId() << endl;
            aguardarEnter();
            break;
        }
        case 2:
        {
            int id;
            cout << "\nEditar Carteira\nID da carteira: ";
            cin >> id;

            Carteira *c = dao.buscarPorId(id);
            if (c)
            {
                string novoNome;
                cout << "Novo nome (" << c->getNomeTitular() << "): ";
                getline(cin >> ws, novoNome);
                c->setNomeTitular(novoNome);
                dao.atualizar(*c);
                cout << "Carteira atualizada!\n";
            }
            else
            {
                cout << "Carteira não encontrada!\n";
            }
            aguardarEnter();
            break;
        }
        case 3:
        {
            int id;
            cout << "\nExcluir Carteira\nID: ";
            cin >> id;
            dao.remover(id);
            cout << "Carteira removida!\n";
            aguardarEnter();
            break;
        }
        case 4:
        {
            cout << "\nLista de Carteiras:\n";
            vector<Carteira> lista = dao.listarTodos();
            for (const auto &c : lista)
            {
                cout << "ID: " << c.getId()
                     << " | Titular: " << c.getNomeTitular()
                     << " | Corretora: " << c.getCorretora() << endl;
            }
            aguardarEnter();
            break;
        }
        }
    } while (opcao != 5);
}

void menuMovimentacao(InMemoryMovimentacaoDAO &movDAO, InMemoryCarteiraDAO &cartDAO, InMemoryOracleDAO &oraDAO)
{
    int opcao;
    do
    {
        system("cls || clear");
        cout << "=== MOVIMENTAÇÃO ==="
             << "\n1. Registrar Compra"
             << "\n2. Registrar Venda"
             << "\n3. Voltar"
             << "\nEscolha: ";
        cin >> opcao;
        cin.ignore();

        if (opcao == 1 || opcao == 2)
        {
            int idCarteira;
            cout << "ID da Carteira: ";
            cin >> idCarteira;

            if (cartDAO.buscarPorId(idCarteira))
            {
                string data;
                double quantidade;
                cout << "Data (AAAA-MM-DD): ";
                cin >> data;
                cout << "Quantidade: ";
                cin >> quantidade;

                Movimentacao mov(idCarteira, data, (opcao == 1) ? 'C' : 'V', quantidade);
                movDAO.criar(mov);

                // Registrar cotação 
                // Cotação aleatória entre 150 e 160
                double cotacao = 150.0 + static_cast<double>(rand()) / RAND_MAX * (160.0 - 150.0);
                oracleDAO.adicionarCotacao(Oracle(data, cotacao));

                cout << "\nOperação registrada com sucesso!\n";
            }
            else
            {
                cout << "Carteira não encontrada!\n";
            }
            aguardarEnter();
        }
    } while (opcao != 3);
}

void menuRelatorios(InMemoryCarteiraDAO &cartDAO, InMemoryMovimentacaoDAO &movDAO, InMemoryOracleDAO &oraDAO)
{
    int opcao;
    do
    {
        system("cls || clear");
        cout << "=== RELATÓRIOS ==="
             << "\n1. Listar Carteiras por ID"
             << "\n2. Listar Carteiras por Nome"
             << "\n3. Saldo de uma Carteira"
             << "\n4. Histórico de Movimentações"
             << "\n5. Ganhos/Perdas Totais"
             << "\n6. Voltar"
             << "\nEscolha: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao)
        {
        case 1:
        { // Listar por ID
            system("cls || clear");
            cout << "=== CARTEIRAS (ORDEM DE ID) ===\n";
            vector<Carteira> lista = cartDAO.listarTodos();
            if (lista.empty())
            {
                cout << "Nenhuma carteira cadastrada!\n";
            }
            else
            {
                for (const auto &c : lista)
                {
                    cout << "ID: " << c.getId()
                         << " | Titular: " << c.getNomeTitular()
                         << " | Corretora: " << c.getCorretora() << endl;
                }
            }
            aguardarEnter();
            break;
        }

        case 2:
        { // Listar por Nome
            system("cls || clear");
            cout << "=== CARTEIRAS (ORDEM ALFABÉTICA) ===\n";
            vector<Carteira> lista = cartDAO.listarTodos();
            if (lista.empty())
            {
                cout << "Nenhuma carteira cadastrada!\n";
            }
            else
            {
                // Ordenar por nome
                sort(lista.begin(), lista.end(),
                     [](const Carteira &a, const Carteira &b)
                     {
                         return a.getNomeTitular() < b.getNomeTitular();
                     });

                for (const auto &c : lista)
                {
                    cout << "Titular: " << left << setw(20) << c.getNomeTitular()
                         << " | ID: " << setw(4) << c.getId()
                         << " | Corretora: " << c.getCorretora() << endl;
                }
            }
            aguardarEnter();
            break;
        }

        case 3:
        { // Saldo da Carteira
            system("cls || clear");
            cout << "=== SALDO DA CARTEIRA ===\n";
            int id;
            cout << "ID da Carteira: ";
            cin >> id;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            Carteira *c = cartDAO.buscarPorId(id);
            if (c)
            {
                vector<Movimentacao> movs = movDAO.listarPorCarteira(id);
                double saldoQuantidade = 0.0;

                for (const auto &m : movs)
                {
                    if (m.getTipoOperacao() == 'C')
                    {
                        saldoQuantidade += m.getQuantidade();
                    }
                    else
                    {
                        saldoQuantidade -= m.getQuantidade();
                    }
                }

                // Pegar última cotação disponível
                // Buscar cotação do dia de hoje
                time_t t = time(nullptr);
                tm* now = localtime(&t);
                char dataHoje[11];
                strftime(dataHoje, sizeof(dataHoje), "%Y-%m-%d", now);

                double cotacaoHoje = oraDAO.buscarCotacaoPorData(dataHoje);

                cout << "\nSaldo em Quantidade: " << saldoQuantidade << " FT\n"
                     << "Valor Atual (Cotação de hoje): R$ "
                     << fixed << setprecision(2) << (saldoQuantidade * cotacaoHoje)
                     << endl;
            }
            else
            {
                cout << "Carteira não encontrada!\n";
            }
            aguardarEnter();
            break;
        }

        case 4:
        { // Histórico de Movimentações
            system("cls || clear");
            cout << "=== HISTÓRICO DE MOVIMENTAÇÕES ===\n";
            int id;
            cout << "ID da Carteira: ";
            cin >> id;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            Carteira *c = cartDAO.buscarPorId(id);
            if (c)
            {
                vector<Movimentacao> movs = movDAO.listarPorCarteira(id);
                if (movs.empty())
                {
                    cout << "Nenhuma movimentação registrada!\n";
                }
                else
                {
                    cout << "\n"
                         << left
                         << setw(12) << "Data"
                         << setw(8) << "Tipo"
                         << setw(10) << "Quantidade"
                         << "Valor (R$)\n";
                    cout << string(50, '-') << endl;

                    for (const auto &m : movs)
                    {
                        double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
                        cout << setw(12) << m.getDataOperacao()
                             << setw(8) << (m.getTipoOperacao() == 'C' ? "Compra" : "Venda")
                             << setw(10) << fixed << setprecision(4) << m.getQuantidade()
                             << "R$ " << fixed << setprecision(2)
                             << (m.getQuantidade() * cotacao) << endl;
                    }
                }
            }
            else
            {
                cout << "Carteira não encontrada!\n";
            }
            aguardarEnter();
            break;
        }

        case 5:
        { // Ganhos/Perdas Totais
            system("cls || clear");
            cout << "=== GANHOS/PERDAS TOTAIS ===\n";
            vector<Carteira> carteiras = cartDAO.listarTodos();
            vector<Oracle> cotacoes = oraDAO.listarTodasCotacoes();

            if (cotacoes.empty())
            {
                cout << "Nenhuma cotação registrada!\n";
                aguardarEnter();
                break;
            }

            double ultimaCotacao = cotacoes.back().getCotacao();

            for (const auto &c : carteiras)
            {
                vector<Movimentacao> movs = movDAO.listarPorCarteira(c.getId());
                double totalInvestido = 0.0;
                double saldoQuantidade = 0.0;

                for (const auto &m : movs)
                {
                    double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
                    if (m.getTipoOperacao() == 'C')
                    {
                        totalInvestido += m.getQuantidade() * cotacao;
                        saldoQuantidade += m.getQuantidade();
                    }
                    else
                    {
                        totalInvestido -= m.getQuantidade() * cotacao;
                        saldoQuantidade -= m.getQuantidade();
                    }
                }

                double valorAtual = saldoQuantidade * ultimaCotacao;
                double resultado = valorAtual - totalInvestido;

                cout << "\nCarteira ID: " << c.getId()
                     << " | Titular: " << c.getNomeTitular()
                     << "\nInvestimento Total: R$ " << fixed << setprecision(2) << totalInvestido
                     << "\nValor Atual: R$ " << valorAtual
                     << "\nResultado: R$ " << resultado
                     << " (" << (resultado >= 0 ? "Lucro" : "Prejuízo") << ")"
                     << "\n"
                     << string(40, '-') << endl;
            }
            aguardarEnter();
            break;
        }
        }
    } while (opcao != 6);
}

void exibirAjuda()
{
    system("cls || clear");
    cout << "=== AJUDA ===\n"
         << "1. Carteira: Gerencie suas carteiras digitais\n"
         << "2. Movimentação: Registre compras e vendas\n"
         << "3. Relatórios: Acesse dados consolidados\n"
         << "4. Ajuda: Exibe esta tela\n"
         << "5. Créditos: Informações sobre o projeto\n";
    aguardarEnter();
}

void exibirCreditos()
{
    system("cls || clear");
    cout << "=== CRÉDITOS ===\n"
         << "Sistema FT Coin\n"
         << "Versão 1.0\n"
         << "Desenvolvido por: Daniel Rosell, Gabriel Sorensen, Rafael Ozorio, Lucas Alberto Amaral, João Pedro Mamede\n"
         << "Copyright © 2023\n";
    aguardarEnter();
}