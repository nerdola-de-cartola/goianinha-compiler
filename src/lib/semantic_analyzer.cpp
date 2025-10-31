#include "semantic_analyzer.hpp"
#include "scope_stack.hpp"
#include <iostream>
#include "error.hpp"

auto default_pos = yy::position(nullptr, 42, 49);
auto default_loc = yy::Parser::location_type(default_pos);

SemanticAnalyzer::SemanticAnalyzer(LexicalAnalyzer &lexer) 
    : lexer(lexer), syn(SyntacticAnalyzer(&lexer)) {}

SemanticAnalyzer::~SemanticAnalyzer() {}

Variable *get_var(Node *node, ScopeStack *stack) {
    Variable *var = stack->get_variable(node->lexeme);
    
    if(var == nullptr) {
        show_error(semantc, default_loc, std::string("undeclared variable"));
    }

    return var;
}

Function *get_func(Node *node, ScopeStack *stack) {
    Function *f = stack->get_function(node->lexeme);
    
    if(f == nullptr) {
        show_error(semantc, default_loc, std::string("undeclared function"));
    }

    return f;
}

void transverse_decl_var(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) return;

    if(node->type == var) {
        stack->add_variable(Variable(node->lexeme, type));
        return;
    }

    transverse_decl_var(node->left, stack, type);
    transverse_decl_var(node->right, stack, type);
}

void transverse_expr(Node *node, ScopeStack *stack, VariableTypes type) {
    if(node == nullptr) return;
    
    transverse_expr(node->left, stack, type);
    transverse_expr(node->right, stack, type);
    
    VariableTypes t = TNULL;
    if(node->var_type != nullptr) t = *node->var_type;
    if(node->type == var) t = get_var(node, stack)->get_type();
    if(node->type == func_call) t = get_func(node, stack)->get_return_type();

    //std::cout << t << std::endl;

    if(t == TNULL) return;
    if(t != type) return show_error(semantc, default_loc, std::string("wrong type"));
}

void transverse(Node *node, ScopeStack *stack) {
    if(node == nullptr) return;

    if(node->type == block) {
        stack->push();
    }

    if(node->type == list_decl_var) {
        transverse_decl_var(node->left, stack, *node->var_type);
    }

    transverse(node->left, stack);
    transverse(node->right, stack);

    if(node->type == block) {
        //stack->pop();
        return;
    }

    if (node->type == add_op || node->type == sub_op || node->type == mul_op || node->type == div_op) {
        transverse_expr(node, stack, INT);
    }
    
    if(node->type == assign_op) {
        auto var = get_var(node, stack);
        transverse_expr(node->left, stack, var->get_type());
    }
}

void SemanticAnalyzer::analyze() {
    syn.parse();
    Node *root = lexer.get_ast();

    if (root == nullptr) return;

    root->printTree();

    ScopeStack stack = ScopeStack();

    transverse(root, &stack);
    std::cout << stack.toString() << std::endl;
}

