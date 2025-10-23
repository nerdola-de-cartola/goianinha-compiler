#include "semantic_analyzer.hpp"
#include "scope_stack.hpp"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(LexicalAnalyzer &lexer) 
    : lexer(lexer), syn(SyntacticAnalyzer(&lexer)) {}

SemanticAnalyzer::~SemanticAnalyzer() {}

VariableTypes current_type = TNULL;

void transverse(Node *node, ScopeStack *stack) {
    if(node->type == block) {
        stack->push();
    }

    if(node->type == list_decl_var) {
        current_type = *node->var_type;
    }

    if(node->left != nullptr) transverse(node->left, stack);

    if(node->type == list_decl_var) {
        current_type = TNULL;
    }

    if(node->right != nullptr) transverse(node->right, stack);

    if(node->type == block) {
        //stack->pop();
        return;
    }


    if(node->type == var) {
        stack->add_variable(Variable(node->lexeme, current_type));
        return;
    }
    
    
    if(node->type == assign_op) {
        //std::cout << "oi" << std::endl;
        Variable *var = stack->get_variable(node->lexeme);
        
        if(var == nullptr) {
            std::cout << "ERROR: undeclared variable\n";
            return;
        }
        
        if(var->get_type() != *node->left->var_type) {
            std::cout << "ERROR: invalid assignment\n";
            return;
        }
    }
}

void SemanticAnalyzer::analyze() {
    syn.parse();
    Node *root = lexer.get_ast();

    if (root == nullptr) return;

    root->printTree();

    ScopeStack stack = ScopeStack();

    transverse(root, &stack);
}

