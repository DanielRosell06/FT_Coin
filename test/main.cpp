#include "View/InterfaceUsuario.hpp"

int main() {
    // Inicializar DAOs
    InMemoryCarteiraDAO carteiraDAO;
    InMemoryMovimentacaoDAO movimentacaoDAO;
    InMemoryOracleDAO oracleDAO;

    // Criar e iniciar interface
    InterfaceUsuario interface(carteiraDAO, movimentacaoDAO, oracleDAO);
    interface.iniciar();

    return 0;
}