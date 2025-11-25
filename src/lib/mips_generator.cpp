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

unsigned long int COND_COUNT = 0;
unsigned long int LOOP_COUNT = 0;

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

std::string get_op_from_node(Node *node) {
    switch (node->type) {
        case add_op:
            return "add";
        case sub_op:
            return "sub";
        case mul_op:
            return "mul";
        case div_op:
            return "div";
        default:
            return "";
    }

    return "";
}

void save_register(std::string reg) {
    generator.add_operation("sw " + reg + ", 0($sp)"); // Save reg into the top of the stack
    generator.add_operation("addiu $sp, $sp, -4"); // Push stack
}

void load_register(std::string reg) {
    generator.add_operation("lw " + reg + ", 4($sp)"); // Load the top of the stack into reg
    generator.add_operation("addiu $sp, $sp, 4"); // Pop stack
}

int generate_expr(Node *node, ScopeStack *stack) {
    if(node == nullptr) return 1;

    if(node->type == number) {
        generator.add_operation("li $s0, " + node->lexeme);
        return 0;
    }

    if(node->type == character) {
        generator.add_operation("li $s0, \'" + node->lexeme + "\'");
        return 0;
    }

    if(node->type == var) {
        auto [var, scope, pos] = get_var_on_stack(node, stack);
        std::string op = "lw $s0, " + std::to_string(4 + pos * 4) + "($fp)";
        generator.add_operation(op); // Load var on s0
        return 0;
    }

    generate_expr(node->left, stack);
    save_register("$s0");
    generate_expr(node->right, stack);
    load_register("$t1");
    std::string op = get_op_from_node(node);
    generator.add_operation(op + " $s0, $t1, $s0"); // left + right
    
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
    //std::cout << pos << std::endl;
    generate_expr(node->left, stack);
    std::string op = "sw $s0, " + std::to_string(4 + pos * 4) + "($fp)";
    generator.add_operation(op);
}

void recursive_variable_declaration(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) return;

    if(node->type == list_decl_var) {
        type = *node->var_type;
    }

    if(node->type == var) {
        Result r = stack->add_variable(Variable(node->lexeme, type));
    }

    recursive_variable_declaration(node->left, stack, type);
    recursive_variable_declaration(node->right, stack, type);
}

std::string get_write_code(Node *node) {
    switch (node->type) {
        case const_string:
            return "4";
        case character:
            return "11";
        default:
            return "1";
    }
}

void generate_write_cmd(Node *node, ScopeStack *stack) {
    auto wc = get_write_code(node->left);

    if(node->left->type == const_string) {
        generator.data_segments.push_back(node->left->lexeme);
        auto index = generator.data_segments.size() - 1;
        std::string op = "la $a0, msg" + std::to_string(index);
        generator.add_operation(op);
    } else {
        generate_expr(node->left, stack);
        generator.add_operation("move $a0, $s0");
    }
    
    generator.add_operation("li $v0, " + wc);
    generator.add_operation("syscall");
}

void generate_prog(Node *node, ScopeStack *stack) {
    generator.add_operation("main:");
    transverse_code(node->left, stack);
    transverse_code(node->right, stack);
    generator.add_operation("li $v0, 10");
    generator.add_operation("syscall");
}

void generate_decl_var(Node *node, ScopeStack *stack) {
    recursive_variable_declaration(node, stack, *node->var_type);
    int count = stack->get_variable_count();
    int frame_size = 4 + count*4;
    generator.add_operation("sw $fp, ($sp)");   // Save old FP on stack top
    generator.add_operation("addiu $sp, $sp, " + std::to_string(-frame_size)); // Push stack
    generator.add_operation("move $fp, $sp");   // Copy SP to FP
}

std::tuple<std::string, std::string, std::string> get_cond_labels() {
    std::string then_label = "then" + std::to_string(COND_COUNT);
    std::string else_label = "else" + std::to_string(COND_COUNT);
    std::string end_label = "end_if" + std::to_string(COND_COUNT);
    COND_COUNT++;
    return {then_label, else_label, end_label};
}

std::tuple<std::string, std::string, std::string> get_loop_labels() {
    std::string run_label = "run" + std::to_string(LOOP_COUNT);
    std::string while_label = "while" + std::to_string(LOOP_COUNT);
    std::string end_label = "end_loop" + std::to_string(LOOP_COUNT);
    LOOP_COUNT++;
    return {run_label, while_label, end_label};
}

std::string get_branch_op(Node *node) {
    switch (node->type) {
        case eq_op:
            return "beq";
        case dif_op:
            return "bne";
        case less_op:
            return "blt";
        case greater_op:
            return "bgt";
        case le_op:
            return "ble";
        case ge_op:
            return "bge";
        
        default:
            return "";
    }
}

std::string generate_cond_expr(Node *node, ScopeStack *stack) {
    auto op = get_branch_op(node);

    if(op == "") { // If there is no comparison in the condition if(x) 
        op = "bne";
        generate_expr(node, stack);
        generator.add_operation("li $t1, 0");
    } else { // If there is a comparison in the condition if (x == y)
        generate_expr(node->left, stack);
        save_register("$s0");
        generate_expr(node->right, stack);
        load_register("$t1");
        // Right on s0 and left on t1
    }

    return op;
}

void generate_conditions(Node *node, ScopeStack *stack) {
    auto condition = node->left;
    auto blocks = node->right;

    auto [then_label, else_label, end_label] = get_cond_labels();
    auto op = generate_cond_expr(condition, stack);

    generator.add_operation(op + " $t1, $s0, " + then_label);

    if(blocks->type != block) { // If and else
        generator.add_operation(else_label + ":");
        transverse_code(blocks->right, stack); // Else
        blocks = blocks->left; // Put then block on blocks variable
    }

    generator.add_operation("b " + end_label);
    generator.add_operation(then_label + ":");
    transverse_code(blocks, stack);
    generator.add_operation(end_label + ":");
}

void generate_new_line(Node *node, ScopeStack *stack) {
    generator.data_segments.push_back("\n");
    auto index = generator.data_segments.size() - 1;
    std::string op = "la $a0, msg" + std::to_string(index);
    generator.add_operation(op);
    generator.add_operation("li $v0, 4");
    generator.add_operation("syscall");
    transverse_code(node->left, stack);
    transverse_code(node->right, stack);
}

void generate_loop(Node *node, ScopeStack *stack) {
    auto condition = node->left;
    auto block = node->right;

    auto [run_label, while_label, end_label] = get_loop_labels();
    
    generator.add_operation(while_label + ":");
    auto op = generate_cond_expr(condition, stack);

    generator.add_operation(op + " $t1, $s0, " + run_label);
    generator.add_operation("b " + end_label);
    generator.add_operation(run_label + ":");
    transverse_code(block, stack);
    generator.add_operation("b " + while_label);
    generator.add_operation(end_label + ":");
}

void transverse_code(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if (node->type == prog) return generate_prog(node, stack);
    if (node->type == list_decl_var) return generate_decl_var(node, stack);
    if (node->type == assign_op) return generate_assign_op(node, stack);
    if (node->type == write_cmd) return generate_write_cmd(node, stack);
    if (node->type == if_cond) return generate_conditions(node, stack);
    if (node->type == new_line) return generate_new_line(node, stack);
    if (node->type == loop) return generate_loop(node, stack);

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
    int line_count = 1;
    std::string buffer;
    bool in_const_str = false;

    for (long unsigned int i = 0; i < code.size(); i++) {
        if(code[i] == '\"') {
            in_const_str = !in_const_str;
        }

        if(code[i] == '\n') {
            if (in_const_str) {
                buffer.push_back('\\');
                buffer.push_back('n');
                continue;
            } else {
                buffer.push_back('\n');
                buffer.append(std::to_string(line_count) + ":\t");
                line_count++;
                continue;
            }
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