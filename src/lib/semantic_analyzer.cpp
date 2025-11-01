#include "semantic_analyzer.hpp"
#include "scope_stack.hpp"
#include "error.hpp"
#include "variable.hpp"
#include "function.hpp"
#include <iostream>

auto default_pos = yy::position(nullptr, 42, 49);
auto default_loc = yy::Parser::location_type(default_pos);

void transverse(Node *node, ScopeStack *stack);
void transverse_func_call(Node *node, ScopeStack *stack);

SemanticAnalyzer::SemanticAnalyzer(LexicalAnalyzer &lexer) 
    : lexer(lexer), syn(SyntacticAnalyzer(&lexer)) {}

SemanticAnalyzer::~SemanticAnalyzer() {}

Variable *get_var(Node *node, ScopeStack *stack) {
    Variable *var = stack->get_variable(node->lexeme);
    
    if(var == nullptr) {
        show_error(semantc, default_loc, "undeclared variable " + node->lexeme, stack);
    }

    return var;
}

Function *get_func(Node *node, ScopeStack *stack) {
    Function *f = stack->get_function(node->lexeme);
    
    if(f == nullptr) {
        show_error(semantc, default_loc, "undeclared function", stack);
    }

    return f;
}

void transverse_decl_var(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) return;

    if(node->type == list_decl_var) {
        type = *node->var_type;
    }

    if(node->type == var) {
        //std::cout << type << " " << node->lexeme << std::endl;
        Result r = stack->add_variable(Variable(node->lexeme, type));
        if(r == ERROR) show_error(semantc, default_loc, "repeated variable name " + node->lexeme + " in block", stack);
        return;
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

void transverse_expr(Node *node, ScopeStack *stack, VariableTypes expected_type) {
    if(node == nullptr) return;
    
    transverse_expr(node->left, stack, expected_type);
    transverse_expr(node->right, stack, expected_type);
    
    VariableTypes current_type = get_node_type(node, stack);

    if(current_type == TNULL) return;

    if(current_type != expected_type) return show_error(
        semantc,
        default_loc,
        "wrong type " + Variable::typeToString(current_type) + " where was expected " + Variable::typeToString(expected_type),
        stack
    );
}

void add_all_parameters(Node *node, Function &f) {
    if(node == nullptr) return;

    if(node->type == param) {
        Variable var = Variable(node->lexeme, *node->var_type);
        Result r = f.add_parameter(var);
        if (r == ERROR) show_error(
            semantc,
            default_loc,
            "reapeted parameter name " + var.get_name() + " in function " + f.get_name()
        );
    }

    add_all_parameters(node->left, f);
    add_all_parameters(node->right, f);
}

//std::cout << "oi" << std::endl;
//std::cout << "ola" << std::endl;

void transverse_function_declaration(Node *node, ScopeStack *stack) {
    auto f1 = node;
    auto f2 = node->left;
    auto list_params = f2->left;
    auto f = Function(f1->lexeme, *f1->var_type);
    add_all_parameters(list_params, f);
    Result r = stack->add_function(f);
    if(r == ERROR) show_error(semantc, default_loc, "repeated function name " + f.get_name(), stack);
    
    for (auto &param : f) { // Add function parameter to the stack
        stack->add_variable(param);
    }

    return transverse(f2->right, stack);
}

void transverse_block(Node *node, ScopeStack *stack) {
    stack->push();
    //std::cout << std::endl << stack->toString() << std::endl;
    transverse(node->left, stack);
    transverse(node->right, stack);
    stack->pop();
    //std::cout << std::endl << stack->toString() << std::endl;
    return;
}

void transverse_assign_op(Node *node, ScopeStack *stack) {
    auto var = get_var(node, stack);
    return transverse_expr(node->left, stack, var->get_type());
}

void transverse_func_call(Node *node, ScopeStack *stack) {
    auto f = get_func(node, stack);

    Node *n = node->left;
    auto params  = f->getParameters();
    int i = params.size() - 1;
    bool finished = false;

    while (i >= 0) {
        //std::cout << "oi" << std::endl;
        //std::cout << "ola" << std::endl;
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

    if (i != -1)
        show_error(
            semantc,
            default_loc,
            "missing parameter " + params[params.size() - i - 1].get_name() + " on call to " + f->get_name(),
            stack
        );

    if (!finished && !(params.size() == 0 && n == nullptr))
        show_error(
            semantc,
            default_loc,
            "extra parameter on call to " + f->get_name(),
            stack
        );
}

void transverse(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if (node->type == block) return transverse_block(node, stack);
    if (node->type == list_decl_var) return transverse_decl_var(node, stack, *node->var_type);
    if (node->type == func1) return transverse_function_declaration(node, stack);
    if (node->type == assign_op) return transverse_assign_op(node, stack);
    if (node->type == func_call) return transverse_func_call(node, stack);
    if (node->type == add_op || node->type == sub_op || node->type == mul_op || node->type == div_op)
        return transverse_expr(node, stack, INT);

    transverse(node->left, stack);
    transverse(node->right, stack);
}

void SemanticAnalyzer::analyze() {
    syn.parse();
    Node *root = lexer.get_ast();

    if (root == nullptr) return;

    ScopeStack stack = ScopeStack();
    stack.push(); // Global scope
    
    //auto a = get_node_type(root, &stack);
    //std::cout << root->type << std::endl;
    //std::cout << Variable::typeToString(a) << std::endl;

    root->printTree();

    transverse(root, &stack);
    std::cout << stack.toString() << std::endl;
}

