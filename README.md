# Compiler Project

A C++ compiler framework for educational and development purposes.

## Project Structure

```
compiler/
├── CMakeLists.txt          # Build configuration
├── README.md              # This file
├── .gitignore             # Git ignore patterns
├── src/                   # Source files
│   └── main.cpp          # Main entry point
├── include/              # Header files
├── tests/               # Test files
└── build/              # Build directory (generated)
```

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- Make (or Ninja build system)

## Building the Project

1. Create a build directory and navigate to it:
   ```bash
   cd compiler
   mkdir build
   cd build
   ```

2. Generate build files:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   make
   ```

Alternatively, you can build directly from the project root:
```bash
cd compiler
cmake -B build
cmake --build build
```

## Running

After building, run the compiler:
```bash
./build/compiler <input_file>
```

Example:
```bash
./build/compiler example.txt
```

## Development

The compiler framework includes the following main components (to be implemented):

- **Lexical Analysis**: Tokenization of source code
- **Parsing**: Syntax analysis and AST generation  
- **Semantic Analysis**: Type checking and symbol table management
- **Code Generation**: Target code generation

## Contributing

1. Add source files to the `src/` directory
2. Add header files to the `include/` directory
3. Add tests to the `tests/` directory
4. Update this README as needed

## License

This project is for educational purposes.
