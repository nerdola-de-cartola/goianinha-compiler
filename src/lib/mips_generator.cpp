#include "mips_generator.hpp"
#include <fstream>
#include <unistd.h>   // for mkstemp, close, unlink
#include <cstdlib>    // for system()

auto def_code = 
        ".data\n"
        "msg: .asciiz \"Hello, world\n\"\n"
        ".text\n"
        ".globl main\n"
        "main:\n"
        "    la   $a0, msg\n"
        "    li   $v0, 4\n"
        "    syscall\n"
        "    li   $v0, 10\n"
        "    syscall\n";


MipsGenerator::MipsGenerator() {}
MipsGenerator::~MipsGenerator() {}

auto generator = MipsGenerator();

void transverse_code(Node *node) {
    if(node == nullptr) return;

    if (node->type == const_string) {
        generator.data_segments.push_back(node->lexeme);
    }

    transverse_code(node->left);
    transverse_code(node->right);
}


std::string data_section() {
    std::string buffer = ".data\n";

    unsigned long int i = 0;
    for (auto data_segment : generator.data_segments) {
        buffer += "msg" + std::to_string(i) + ": .asciiz \"" + data_segment + "\"\n";
        i++;
    }

    return buffer;
}

std::string generate_code(Node *node) {
    std::string buffer;
    transverse_code(node);
    buffer += data_section();
    //std::cout << buffer << std::endl << std::endl << std::endl;
    return def_code;
    return buffer;
}

void print_code(const std::string &code) {
    std::string buffer;
    bool in_const_str = false;

    for (long unsigned int i = 0; i < code.size(); i++) {
        if(code[i] == '\"') {
            in_const_str = !in_const_str;
        }

        if(code[i] == '\n' && in_const_str) {
            buffer.push_back('\\');
            buffer.push_back('n');
            continue;
        }
        
        buffer.push_back(code[i]);
    }

    std::cout <<
        "======================================================\n" <<
        buffer <<
        "======================================================\n" <<
        std::endl;
}

void run_code(const std::string& code) {
    // Create a temporary file template
    char tmpFileTemplate[] = "mipsXXXXXX";

    // mkstemp creates the file and opens it
    int fd = mkstemp(tmpFileTemplate);
    if (fd == -1) {
        std::cerr << "Failed to create temporary file.\n";
        return;
    }

    // Write content into the file
    std::ofstream tmpFile(tmpFileTemplate);
    if (!tmpFile) {
        std::cerr << "Failed to write to temporary file.\n";
        close(fd);
        unlink(tmpFileTemplate);
        return;
    }

    tmpFile << code;
    tmpFile.close();
    close(fd);

    // Build and run the spim command
    std::string cmd = "spim -file ";
    cmd += tmpFileTemplate;
    std::cout << "Running: " << cmd << "\n";
    system(cmd.c_str());

    // Remove the temporary file after execution
    unlink(tmpFileTemplate);
}