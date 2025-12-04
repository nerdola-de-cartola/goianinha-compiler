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
const long int POS_FOR_GLOBALS = 687;
long int CURRENT_VARIABLE_POSITION = 0;
long int CURRENT_PARAMETER_POSITION = +12;
Function MAIN = Function("main", INT);
Function *CURRENT_FUNC = nullptr;

std::vector<std::string> globals;

void transverse_code(Node *node, ScopeStack *stack);
void generate_block(Node *node, ScopeStack *stack, Function *f);
void generate_func_call(Node *node, ScopeStack *stack);
std::tuple<Variable *, int, int> get_var_on_stack(Node *node, ScopeStack *stack);

MipsGenerator::MipsGenerator() {}
MipsGenerator::~MipsGenerator() {}

int MipsGenerator::add_operation(std::string op) {
    op.append("\n");
    operations.push_back(op);
    return 0;
}

auto generator = MipsGenerator();

bool isGlobal(Variable &var) {
    if (var.pos == POS_FOR_GLOBALS) 
        return true;

    return false;
}

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
        case negative_op:
            return "neg";
        default:
            return "NOOP";
    }
}

void save_register(std::string reg) {
    generator.add_operation("sw " + reg + ", 0($sp)"); // Save reg into the top of the stack
    generator.add_operation("addiu $sp, $sp, -4"); // Push stack
}

void load_register(std::string reg) {
    generator.add_operation("lw " + reg + ", 4($sp)"); // Load the top of the stack into reg
    generator.add_operation("addiu $sp, $sp, 4"); // Pop stack
}

void generate_expr(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if(node->type == number) {
        std::string value = node->lexeme;
        generator.add_operation("li $s0, " + value);
        return;
    }

    if(node->type == func_call) {
        generate_func_call(node, stack);
        return;
    }

    if(node->type == character) {
        generator.add_operation("li $s0, \'" + node->lexeme + "\'");
        return;
    }

    if(node->type == var) {
        auto [var, scope, pos] = get_var_on_stack(node, stack);
        int offset = var->pos;
        //std::cout << var->toString() << " na posição " << var->pos << std::endl;

        if(isGlobal(*var)) { // Global variables
            //std::cout << "Acessando global" << std::endl;
            generator.add_operation("la $t0, " + var->get_name()); // Load global addrs on t0
            generator.add_operation("lw $s0, 0($t0)"); // Load global value on s0
        } else { // Local variables
            generator.add_operation("lw $s0, " + std::to_string(offset) + "($fp)"); // Load var on s0
        }

        return;
    }

    generate_expr(node->left, stack);
    save_register("$s0");
    generate_expr(node->right, stack);
    load_register("$t1");
    std::string op = get_op_from_node(node);

    if(node->type == negative_op) generator.add_operation(op + " $s0, $t1"); // s0 = -left
    else generator.add_operation(op + " $s0, $t1, $s0"); // s0 = left op right
    
    return;
}

std::tuple<Variable *, int, int> get_var_on_stack(Node *node, ScopeStack *stack) {
    int pos;
    int scope;
    auto var = stack->get_variable(node->lexeme, &scope, &pos);
    return {var, scope, pos};
}

void generate_assign_op(Node *node, ScopeStack *stack) {
    if (node == nullptr) return;

    auto [var, scope, pos] = get_var_on_stack(node, stack);
    //std::cout << var->toString() << " no escopo " << scope << std::endl;

    if (node->left->type == assign_op) generate_assign_op(node->left, stack);
    else generate_expr(node->left, stack);

    if (isGlobal(*var)) { // Global variables
        generator.add_operation("la $t0, " + var->get_name());        
        generator.add_operation("sw $s0, 0($t0)");        
    } else { // Local variable
        generator.add_operation("sw $s0, " + std::to_string(var->pos) + "($fp)");
    }
}

void recursive_variable_declaration(Node *node, ScopeStack *stack, VariableTypes type, int *variables_count) {
    if(node == nullptr) return;

    if(node->type == list_decl_var) {
        type = *node->var_type;
    }

    if(node->type == var) {
        if (CURRENT_FUNC == nullptr) { // Global variables
            stack->add_variable(Variable(node->lexeme, type, POS_FOR_GLOBALS));
            globals.push_back(node->lexeme);
        } else { // Local variables
            stack->add_variable(Variable(node->lexeme, type, CURRENT_VARIABLE_POSITION));
            CURRENT_VARIABLE_POSITION += -4;
            (*variables_count)++;
        }
    }

    recursive_variable_declaration(node->left, stack, type, variables_count);
    recursive_variable_declaration(node->right, stack, type, variables_count);
}

VariableTypes get_type_from_node(Node *node, ScopeStack *stack) {
    if(node->var_type != nullptr) return *node->var_type;

    if(node->type == var) {
        return stack->get_variable(node->lexeme)->get_type();
    }

    return INT;
}

std::string get_write_code(VariableTypes type) {
    switch (type) {
        case STR:
            return "4";
        case CAR:
            return "11";
        case INT:
            return "1";
        default:
            return "NO_CODE_FOR_WRITE";
    }
}

std::string get_read_code(VariableTypes type) {
    switch (type) {
        case CAR:
            return "12";
        case INT:
            return "5";
        default:
            return "NO_CODE_FOR_READ";
    }
}

void generate_read_cmd(Node *node, ScopeStack *stack) {
    auto var = stack->get_variable(node->lexeme);
    auto wc = get_read_code(var->get_type());
    generator.add_operation("li $v0, " + wc);
    generator.add_operation("syscall");
    generator.add_operation("sw $v0, " + std::to_string(var->pos) + "($fp)"); // Save the read value into stack
}

void generate_write_cmd(Node *node, ScopeStack *stack) {
    auto wc = get_write_code(get_type_from_node(node->left, stack));

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
    transverse_code(node->left, stack);

    CURRENT_FUNC = &MAIN;
    generator.add_operation("main:");
    generator.add_operation("move $s1, $sp"); // Copy sp to s1
    generator.add_operation("move $fp, $sp"); // Copy sp to fp
    generate_block(node->right, stack, nullptr);
    generator.add_operation("li $v0, 10"); // Call exit(0)
    generator.add_operation("syscall");
    CURRENT_FUNC = nullptr;
}

void generate_decl_var(Node *node, ScopeStack *stack) {
    int variables_count = 0;
    recursive_variable_declaration(node, stack, *node->var_type, &variables_count);

    if (variables_count != 0) {
        generator.add_operation("addiu $sp, $sp, " + std::to_string(-(variables_count*4))); // Push stack
    }
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

void add_params_to_stack(Node *node, Function &f) {
    if(node == nullptr) return;

    if(node->type == param) {
        Variable var = Variable(node->lexeme, *node->var_type, CURRENT_PARAMETER_POSITION);
        CURRENT_PARAMETER_POSITION += 4;
        f.add_parameter(var);
    }

    add_params_to_stack(node->left, f);
    add_params_to_stack(node->right, f);
}

void generate_block(Node *node, ScopeStack *stack, Function *f) {
    stack->push();
    int previous_variable_start = CURRENT_VARIABLE_POSITION;


    if (f != nullptr) {
        for (auto &param : *f) { // Add function parameter to the stack
            stack->add_variable(param);
        }
    }

    transverse_code(node->left, stack);
    transverse_code(node->right, stack);
    
    // Reset stack position for local variables
    CURRENT_VARIABLE_POSITION = previous_variable_start; 
    stack->pop();
    return;
}

void generate_func(Node *node, ScopeStack *stack) {
    auto f1 = node;
    auto f2 = node->left;
    auto list_params = f2->left;
    auto f = Function(f1->lexeme, *f1->var_type);
    CURRENT_FUNC = &f;
    add_params_to_stack(list_params, f);
    
    stack->add_function(f);
    
    generator.add_operation(f.get_name() + ":");
    save_register("$fp");
    save_register("$ra");
    generator.add_operation("move $fp, $sp"); // Move sp to fp

    CURRENT_PARAMETER_POSITION = +12;
    generate_block(f2->right, stack, &f);
    CURRENT_PARAMETER_POSITION = +12;
    
    generator.add_operation("move $sp, $fp"); // Restore sp for caller, this also mean popping local variables
    load_register("$ra");
    load_register("$fp");
    int offset = f.getParameters().size() * 4;
    generator.add_operation("addiu $sp, $sp, " + std::to_string(offset)); // Pop parameters
    generator.add_operation("jr $ra"); // Jump to caller
    CURRENT_FUNC = nullptr;
}

void generate_func_call(Node *node, ScopeStack *stack) {
    //std::cout << "oiiiiiiiila" << std::endl;
    auto f = stack->get_function(node->lexeme);

    Node *n = node->left;
    auto params  = f->getParameters();

    while (true) {
        if (n->type == list_exp) {
            generate_expr(n->right, stack);
            save_register("$s0");
        } else {
            generate_expr(n, stack);
            save_register("$s0");
            break;
        }

        n = n->left;
    }

    generator.add_operation("jal " + node->lexeme);
}

void generate_return(Node *node, ScopeStack *stack) {
    generate_expr(node->left, stack);
}

void transverse_code(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if (node->type == prog) return generate_prog(node, stack);
    if (node->type == func1) return generate_func(node, stack);
    if (node->type == list_decl_var) return generate_decl_var(node, stack);
    if (node->type == assign_op) return generate_assign_op(node, stack);
    if (node->type == read_cmd) return generate_read_cmd(node, stack);
    if (node->type == write_cmd) return generate_write_cmd(node, stack);
    if (node->type == if_cond) return generate_conditions(node, stack);
    if (node->type == new_line) return generate_new_line(node, stack);
    if (node->type == loop) return generate_loop(node, stack);
    if (node->type == func_call) return generate_func_call(node, stack);
    if (node->type == block) return generate_block(node, stack, nullptr);
    if (node->type == return_cmd) return generate_return(node, stack);

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

    for (auto global : globals) {
        buffer += global + ": .word 0\n";
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

    buffer.append(std::to_string(line_count) + ":\t");
    line_count++;

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

                if (i != code.size()-1) {
                    buffer.append(std::to_string(line_count) + ":\t");
                    line_count++;
                }
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