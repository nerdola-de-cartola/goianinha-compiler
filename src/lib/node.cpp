#include "node.hpp"
#include <iostream>


Node::Node(TypeNode type, Node *left, Node *right, int value)
    : type(type), left(left), right(right), value(value) {}

Node::Node(TypeNode type, Node *left, Node *right)
    : type(type), left(left), right(right) {}

Node::Node(TypeNode type, Node *left)
    : type(type), left(left), right(nullptr) {}

Node::Node(TypeNode type, int value)
    : type(type), left(nullptr), right(nullptr), value(value) {}

Node::Node(TypeNode type, std::string lexeme)
    : type(type), left(nullptr), right(nullptr) {}

Node::Node(TypeNode type, Node *left, std::string lexeme)
    : type(type), left(left), right(nullptr) {}

Node::~Node() {
    delete left;
    delete right;
}


void Node::traverse_prev() {
    
    if (left != nullptr) {
        left->traverse_prev();
    }
    
    if (right != nullptr) {
        right->traverse_prev();
    }
    
    std::cout << '<' << toString() << '>' << '\n';

    return;
}


void Node::traverse_in() {
    if (left != nullptr) {
        left->traverse_in();
    }

    std::cout << '<' << toString() << '>' << '\n';
    
    if (right != nullptr) {
        right->traverse_in();
    }
    

    return;
}


void Node::traverse_pos() {
    if (left != nullptr) {
        left->traverse_pos();
    }
    
    if (right != nullptr) {
        right->traverse_pos();
    }
    
    std::cout << '<' << toString() << '>' << '\n';

    return;
}

void Node::printTree(std::string prefix, bool isLeft) {
    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");
    std::cout << this->toString() << "\n";

    // Prepare prefix for children
    std::string newPrefix = prefix + (isLeft ? "│   " : "    ");

    // Print left and right recursively
    if (this->left || this->right) {
        if (this->left != nullptr)
            this->left->printTree(newPrefix, true);
        else
            std::cout << newPrefix << "├── (null)\n";

        if (this->right != nullptr)
            this->right->printTree(newPrefix, false);
        else
            std::cout << newPrefix << "└── (null)\n";
    }
}


std::string Node::toString() {
    switch (type) {
        case id: return "id";
        case number: return "number";
        case character: return "character";
        case assign_op: return "assign_op";
        case negative_op: return "negative_op";
        case add_op: return "add_op";
        case sub_op: return "sub_op";
        case mul_op: return "mul_op";
        case div_op: return "div_op";
        case or_op: return "or_op";
        case and_op: return "and_op";
        case not_op: return "not_op";
        case less_op: return "less_op";
        case le_op: return "le_op";
        case greater_op: return "greater_op";
        case ge_op: return "ge_op";
        case eq_op: return "eq_op";
        case dif_op: return "dif_op";
        case command: return "command";
        case loop: return "loop";
        case if_cond: return "if_cond";
        case if_blocks: return "if_blocks";
        case write: return "write";
        case read: return "read";
        case return_cmd: return "return_cmd";
        case var: return "var";
        case list_var: return "list_var";
        case list_decl_var: return "list_decl_var";
        case block: return "block";
        case func: return "func";
        case func_params_block: return "func_params_block";
        case func_type_id: return "func_type_id";
        case param: return "param";
        case list_params: return "list_params";
        case decl_func_var: return "decl_func_var";
        case prog: return "prog";
        case list_exp: return "list_exp";
        case exp: return "exp";
    }

    return "";
}