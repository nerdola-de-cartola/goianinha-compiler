#include "semantic_analyzer.hpp"
#include "scope_stack.hpp"
#include "error.hpp"
#include "variable.hpp"
#include "function.hpp"
#include <iostream>

Function *CURRENT_FUNCTION = nullptr;

void transverse(Node *node, ScopeStack *stack);
void transverse_func_call(Node *node, ScopeStack *stack);
void transverse_block(Node *node, ScopeStack *stack, Function *f);

SemanticAnalyzer::SemanticAnalyzer(LexicalAnalyzer &lexer, SyntacticAnalyzer &syn) 
    : lexer(lexer), syn(syn) {}

SemanticAnalyzer::~SemanticAnalyzer() {}

Variable *get_var(Node *node, ScopeStack *stack) {
    Variable *var = stack->get_variable(node->lexeme);
    
    if(var == nullptr) {
        show_error(semantic, node->loc, "undeclared variable " + node->lexeme, stack);
    }

    return var;
}

Function *get_func(Node *node, ScopeStack *stack) {
    Function *f = stack->get_function(node->lexeme);
    
    if(f == nullptr) {
        show_error(semantic, node->loc, "undeclared function", stack);
    }

    return f;
}

void transverse_decl_var(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) {
        return;
    }

    if(node->type == list_decl_var) {
        type = *node->var_type;
    }

    if(node->type == var) {
        Result r = stack->add_variable(Variable(node->lexeme, type));
        if(r == ERROR) {
            //std::cout << node->toString() << std::endl;
            show_error(semantic, node->loc, "repeated variable name " + node->lexeme + " in block", stack);
        }
    }

    transverse_decl_var(node->left, stack, type);
    transverse_decl_var(node->right, stack, type);
}

VariableTypes get_node_type(Node *node, ScopeStack *stack) {
    VariableTypes type = TNULL;
    if(node->var_type != nullptr) type = *node->var_type;
    if(node->type == var) type = get_var(node, stack)->get_type();

    if(node->type == func_call) {
        type = get_func(node, stack)->get_return_type();
        transverse_func_call(node, stack);
    }

    return type;
}

bool isOperation(Node *node) {
    if(node->type == or_op || node->type == and_op || node->type == not_op || node->type == add_op || node->type == sub_op || node->type == mul_op || node->type == div_op || node->type == eq_op || node->type == dif_op || node->type == less_op || node->type == le_op || node->type == greater_op || node->type == ge_op)
        return true;

    return false;
}

// Return the type of a node recursive
// Validate if all operations are between operands of the same type
VariableTypes get_recursive_node_type(Node *node, ScopeStack *stack) {
    if(node == nullptr) return TNULL;

    auto type = get_node_type(node, stack);

    if (type != TNULL) return type;

    if (node->type == assign_op) {
        auto var = get_var(node, stack);
        return var->get_type();
    }

    if(isOperation(node)) {
        VariableTypes lt = get_recursive_node_type(node->left, stack);
        VariableTypes rt = get_recursive_node_type(node->right, stack);

        if (lt != rt) {
            //std::cout << node->toString() << std::endl;
            std::string msg = "different types " + Variable::typeToString(lt) + " and " + Variable::typeToString(rt) + " in operation";
            show_error(semantic, node->loc, msg, stack);
        }


        return INT;
    }

    return TNULL;
}

// Validate if an expression has the expected type
void transverse_expr(Node *node, ScopeStack *stack, VariableTypes expected_type) {
    if(node == nullptr) return;

    auto current_type = get_recursive_node_type(node, stack);

    if(current_type != expected_type) {
        //std::cout << node->toString() << std::endl;
        std::string msg = "wrong type " + Variable::typeToString(current_type) + " where was expected " + Variable::typeToString(expected_type); 
        return show_error(semantic, node->loc, msg, stack);
    }
}

void add_all_parameters(Node *node, Function &f) {
    if(node == nullptr) return;

    if(node->type == param) {
        Variable var = Variable(node->lexeme, *node->var_type);
        Result r = f.add_parameter(var);
        if (r == ERROR) {
            //std::cout << node->toString() << std::endl;
            std::string msg = "repeated parameter name " + var.get_name() + " in function " + f.get_name(); 
            show_error(semantic, node->loc, msg);
        }
    }

    add_all_parameters(node->left, f);
    add_all_parameters(node->right, f);
}

void transverse_function_declaration(Node *node, ScopeStack *stack) {
    auto f1 = node;
    auto f2 = node->left;
    auto list_params = f2->left;
    auto f = Function(f1->lexeme, *f1->var_type);
    add_all_parameters(list_params, f);
    
    Result r = stack->add_function(f);
    if(r == ERROR) {
        //std::cout << node->toString() << std::endl;
        show_error(semantic, node->loc, "repeated function name " + f.get_name(), stack);
    }
    
    CURRENT_FUNCTION = &f;
    transverse_block(f2->right, stack, &f);
    CURRENT_FUNCTION = nullptr;
}

void transverse_block(Node *node, ScopeStack *stack, Function *f) {
    stack->push();

    if (f != nullptr) {
        for (auto &param : *f) { // Add function parameter to the stack
            stack->add_variable(param);
        }
    }

    transverse(node->left, stack);
    transverse(node->right, stack);
    stack->pop();
    return;
}

void transverse_assign_op(Node *node, ScopeStack *stack) {
    auto var = get_var(node, stack);
    transverse_expr(node->left, stack, var->get_type());
    return transverse(node->left, stack);
}

void transverse_func_call(Node *node, ScopeStack *stack) {
    auto f = get_func(node, stack);

    Node *n = node->left;
    auto params  = f->getParameters();
    int i = params.size() - 1;
    bool finished = false;

    while (i >= 0) {
        if (n == nullptr) break;

        if (n->type != list_exp) {
            transverse_expr(n, stack, params[i].get_type());    
            finished = true;
            i--;
            break;
        }

        transverse_expr(n->right, stack, params[i].get_type());
        i--;
        n = n->left;
    }

    if (i != -1) {
        //std::cout << node->toString() << std::endl;
        std::string msg = "missing parameter " + params[params.size() - i - 1].get_name() + " on call to " + f->get_name();
        show_error(semantic, node->loc, msg, stack);
    }

    if (!finished && !(params.size() == 0 && n == nullptr)) {
        //std::cout << node->toString() << std::endl;
        show_error(
            semantic,
            node->loc,
            "extra parameter on call to " + f->get_name(),
            stack
        );
    }   
}

void transverse_loop(Node *node, ScopeStack *stack) {
    get_recursive_node_type(node->left, stack);
    return transverse(node->right, stack);
}

void transverse_condition(Node *node, ScopeStack *stack) {
    get_recursive_node_type(node->left, stack);
    return transverse(node->right, stack);
}

void transverse_return(Node *node, ScopeStack *stack) {
    
    if (CURRENT_FUNCTION == nullptr) 
    return transverse(node->left, stack);
    
    //std::cout << CURRENT_FUNCTION->toString() << std::endl;
    auto type = CURRENT_FUNCTION->get_return_type();
    return transverse_expr(node->left, stack, type);
}

void transverse_read_write_cmd(Node *node, ScopeStack *stack) {
    get_recursive_node_type(node->left, stack);
}

void transverse(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if (node->type == block) return transverse_block(node, stack, nullptr);
    if (node->type == list_decl_var) return transverse_decl_var(node, stack, *node->var_type);
    if (node->type == func1) return transverse_function_declaration(node, stack);
    if (node->type == assign_op) return transverse_assign_op(node, stack);
    if (node->type == func_call) return transverse_func_call(node, stack);
    if (node->type == loop) return transverse_loop(node, stack);
    if (node->type == if_cond) return transverse_condition(node, stack);
    if (node->type == return_cmd) transverse_return(node, stack);
    if (node->type == read_cmd) transverse_read_write_cmd(node, stack);
    if (node->type == write_cmd) transverse_read_write_cmd(node, stack);


    if (isOperation(node)) {
        get_recursive_node_type(node, stack);
        return;
    }

    transverse(node->left, stack);
    transverse(node->right, stack);
}

void SemanticAnalyzer::analyze() {
    Node *root = lexer.get_ast();
    
    if (root == nullptr) {
        syn.parse();
        root = lexer.get_ast();
        if (root == nullptr) return;
    }

    ScopeStack stack = ScopeStack();
    stack.push(); // Global scope
    
    transverse(root, &stack);
}

