#include "mips_generator.hpp"
#include <fstream>
#include <unistd.h>   // for mkstemp, close, unlink
#include <cstdlib>    // for system()


std::string generate_code(Node *root) {
    return 
        ".data @"
        "msg: .asciiz \"Hello, world\n\" @"
        ".text @"
        ".globl main @"
        "main: @"
        "    la   $a0, msg @"
        "    li   $v0, 4 @"
        "    syscall @"
        "    li   $v0, 10 @"
        "    syscall @"
    ;
}

void print_code(const std::string &code) {
    std::string buffer;

    for (long unsigned int i = 0; i < code.size(); i++) {
        //std::cout << code[i] << std::endl;

        if(code[i] == '\n') {
            buffer.push_back('\\');
            buffer.push_back('n');
            continue;
        }

        if(code[i] == '@') {
            buffer.push_back('\n');
            continue;
        }

        buffer.push_back(code[i]);
    }

    std::cout << buffer << std::endl;
}

void run_code(const std::string& code) {
    std::string buffer;

    for (long unsigned int i = 0; i < code.size(); i++) {
        if(code[i] == '@') {
            buffer.push_back('\n');
            continue;
        }
        buffer.push_back(code[i]);
    }

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

    tmpFile << buffer;
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