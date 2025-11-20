#include "mips_generator.hpp"
#include "error.hpp"
#include "scope_stack.hpp"
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

void transverse_code(Node *node, ScopeStack *stack);
std::tuple<Variable *, int, int> get_var_on_stack(Node *node, ScopeStack *stack);

MipsGenerator::MipsGenerator() {}
MipsGenerator::~MipsGenerator() {}

int MipsGenerator::add_operation(std::string op) {
    op.append("\n");
    operations.push_back(op);
    return 0;
}

auto generator = MipsGenerator();

int generate_expr(Node *node, ScopeStack *stack) {
    if(node->type == number) {
        generator.add_operation("li $s0, " + node->lexeme);
        return 0;
    }

    if(node->type == var) {
        auto [var, scope, pos] = get_var_on_stack(node, stack);
        std::string op = "lw $s0, " + std::to_string(4 + pos * 4) + "($sp)";
        generator.add_operation(op); // Load var on s0
    }

    if(node->type == add_op) {
        generate_expr(node->left, stack);
        generator.add_operation("sw $s0, 0($sp)"); // Save left result on stack
        generator.add_operation("addiu $sp, $sp, -4"); // Push stack
        generate_expr(node->right, stack);
        generator.add_operation("lw $t1, 4($sp)"); // Load left result on t1
        generator.add_operation("addiu $sp, $sp, 4"); // Pop stack
        generator.add_operation("add $s0, $s0, $t1"); // Add left and right results
    }
    
    return 0;
}

std::tuple<Variable *, int, int> get_var_on_stack(Node *node, ScopeStack *stack) {
    int pos;
    int scope;
    auto var = stack->get_variable(node->lexeme, &scope, &pos);
    return {var, scope, pos};
}

void generate_assign_op(Node *node, ScopeStack *stack) {
    auto [var, scope, pos] = get_var_on_stack(node, stack);
    std::cout << pos << std::endl;
    generate_expr(node->left, stack);
    std::string op = "sw $s0, " + std::to_string(4 + pos * 4) + "($sp)";
    generator.add_operation(op);
}

void generate_decl_var(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) {
        return;
    }

    if(node->type == list_decl_var) {
        type = *node->var_type;
    }

    if(node->type == var) {
        Result r = stack->add_variable(Variable(node->lexeme, type));
    }

    generate_decl_var(node->left, stack, type);
    generate_decl_var(node->right, stack, type);
}

void generate_write_cmd(Node *node, ScopeStack *stack) {
    if(node->left->type == const_string) {
        generator.data_segments.push_back(node->left->lexeme);
        auto index = generator.data_segments.size() - 1;
        std::string op = "la $a0, msg" + std::to_string(index);
        generator.add_operation(op);
        generator.add_operation("li $v0, 4");
    } else {
        generate_expr(node->left, stack);
        generator.add_operation("move $a0, $s0");
        generator.add_operation("li $v0, 1");
    }

    generator.add_operation("syscall");
}

void generate_prog(Node *node, ScopeStack *stack) {
    generator.add_operation("main:");
    transverse_code(node->left, stack);
    transverse_code(node->right, stack);
    generator.add_operation("li $v0, 10");
    generator.add_operation("syscall");
}

void transverse_code(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if (node->type == prog) return generate_prog(node, stack);

    if (node->type == func1) {
        generator.functions.push_back(node->lexeme);
    }

    if (node->type == list_decl_var) {
        generate_decl_var(node, stack, *node->var_type);
        int count = stack->get_variable_count();
        generator.add_operation("addiu $sp, $sp, " + std::to_string(-(count*4)));
        return;
    }

    if (node->type == assign_op) return generate_assign_op(node, stack);
    if (node->type == write_cmd) return generate_write_cmd(node, stack);

    transverse_code(node->left, stack);
    transverse_code(node->right, stack);
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

std::string text_section() {
    std::string buffer = ".text\n";
    buffer += ".globl main\n";

    for (auto &op : generator.operations) {
        buffer += op;
    }

    return buffer;
}

std::string generate_code(Node *node) {
    std::string buffer;
    ScopeStack stack = ScopeStack();
    stack.push();
    transverse_code(node, &stack);
    stack.pop();
    buffer += data_section();
    buffer += text_section();

    //std::cout << buffer << std::endl << std::endl << std::endl;
    //return def_code;

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