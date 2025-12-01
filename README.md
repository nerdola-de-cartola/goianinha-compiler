# Projeto Compilador

Um compilador para a linguagem Goianinha, escrito em C++ para fins educacionais.

## Estrutura do Projeto

```
compiler/
├── CMakeLists.txt                             # Configuração de build
├── README.md                                  # Este arquivo
├── .gitignore                                 # Padrões de ignore do Git
├── include/                                   # Arquivos de cabeçalho
├── examples/                                  # Arquivos de exemplo para entrada
├── tests/                                     # Arquivos de exemplo para entrada
├── build/                                     # Diretório de build (gerado, arquivos do bison e do flex)
└── src/                                       # Arquivos fonte
    └──lib/                                    # Código de biblioteca
    └── main.cpp                               # Ponto de entrada para o compilador e simulador de mips.
    └── test_ast.cpp                           # Ponto de entrada para teste da árvore abstrata de sintax 
    └── test_lexical_analyzer.cpp              # Ponto de entrada para teste do analizador lexico
    └── test_scope_stack.cpp                   # Ponto de entrada para teste da pilha de escopos
    └── test_script.py                         # Ponto de entrada para testar o compilador com todos os arquivos da pasta tests/
    └── test_semantic_analyzer.cpp             # Ponto de entrada para teste do analizador semântico
    └── test_syntactic_analyzer.cpp            # Ponto de entrada para teste do analizador syntatico
```

## Pré-requisitos

- Compilador compatível com C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 ou superior
- Make
- Flex
- Bison

## Compilando o Projeto

1. Crie um diretório de build e navegue até ele:
   ```bash
   cd compiler
   mkdir -p build/bison
   cd build
   ```

2. Gere os arquivos de build:
   ```bash
   cmake ..
   ```

3. Compile o projeto:
   ```bash
   make
   ```

Alternativamente, você pode compilar diretamente da raiz do projeto:
```bash
cd compiler
cmake -B build
cmake --build build
```

## Executando

Após a compilação, execute o compilador/simulador:
```bash
./build/compiler ./examples/mips/consts.txt
```

## Desenvolvimento

O framework do compilador inclui os seguintes componentes principais (a serem implementados):

- **Análise Léxica**: Tokenização do código fonte
- **Análise Sintática**: Análise da sintaxe e geração de AST  
- **Análise Semântica**: Verificação de tipos e gerenciamento de tabela de símbolos
- **Geração de Código**: Geração de código alvo
- **Simulação**: Execução do código MIPS

## Licença

Este projeto é para fins educacionais.
