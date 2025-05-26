# Como Compilar o Projeto

Para compilar este projeto, utilize o comando abaixo no terminal:

```sh
g++ -Iinclude -std=c++11 test/main.cpp src/Model/Carteira.cpp src/Model/Movimentacao.cpp src/Model/Oracle.cpp src/DAO/InMemoryCarteiraDAO.cpp src/DAO/InMemoryMovimentacaoDAO.cpp src/DAO/InMemoryOracleDAO.cpp src/View/MenuBase.cpp src/View/MenuCarteira.cpp src/View/MenuMovimentacao.cpp src/View/MenuRelatorios.cpp src/View/InterfaceUsuario.cpp -o test/ft_coin
```

- `-Iinclude`: Inclui o diretório de headers.
- `-std=c++11`: Usa o padrão C++11.
- Os arquivos `.cpp` são os módulos do projeto.
- O executável será gerado em `test/main`.

> **Dica:** Certifique-se de que todos os diretórios e arquivos existem conforme o comando acima.
