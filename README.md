# Projeto Compilador

Um framework de compilador C++ para fins educacionais e de desenvolvimento.

## Estrutura do Projeto

```
compiler/
├── CMakeLists.txt                             # Configuração de build
├── README.md                                  # Este arquivo
├── .gitignore                                 # Padrões de ignore do Git
├── src/                                       # Arquivos fonte
|   └──lib/                                    # Código de biblioteca
│   └── main.cpp                               # Ponto de entrada para teste do analizador sintático
│   └── test_lexical_analyzer.cpp              # Ponto de entrada para teste do analizador lexico
│   └── test_scope_stack.cpp                   # Ponto de entrada para teste da pilha de escopos
├── include/                                   # Arquivos de cabeçalho
├── examples/                                  # Arquivos de exemplo para entrada
└── build/                                     # Diretório de build (gerado, arquivos do bison e do flex)
```

## Pré-requisitos

- Compilador compatível com C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 ou superior
- Make (ou sistema de build Ninja)
- Flex
- Bison

## Compilando o Projeto

1. Crie um diretório de build e navegue até ele:
   ```bash
   cd compiler
   mkdir build
   cd build
   mkdir bison
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

Após a compilação, execute os programas de teste:
```bash
./build/compiler examples/1.txt
./build/test_lexical_analyzer examples/1.txt
./build/test_scope_stack
```

## Desenvolvimento

O framework do compilador inclui os seguintes componentes principais (a serem implementados):

- **Análise Léxica**: Tokenização do código fonte
- **Análise Sintática**: Análise da sintaxe e geração de AST  
- **Análise Semântica**: Verificação de tipos e gerenciamento de tabela de símbolos
- **Geração de Código**: Geração de código alvo

## Contribuindo

1. Adicione arquivos fonte ao diretório `src/`
2. Adicione arquivos de cabeçalho ao diretório `include/`
3. Adicione testes ao diretório `tests/`
4. Atualize este README conforme necessário

## Licença

Este projeto é para fins educacionais.
