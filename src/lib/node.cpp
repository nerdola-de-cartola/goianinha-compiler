#include "node.hpp"
#include "variable.hpp"
#include <iostream>

Node::Node(TypeNode type)
    : type(type), left(nullptr), right(nullptr), value(nullptr), var_type(nullptr), lexeme("") {}

Node::Node(TypeNode type, Node *left, Node *right, int value)
    : type(type), left(left), right(right), value(new int(value)), var_type(nullptr), lexeme("") {}

Node::Node(TypeNode type, Node *left, Node *right)
    : type(type), left(left), right(right), value(nullptr), var_type(nullptr), lexeme("") {}

Node::Node(TypeNode type, Node *left)
    : type(type), left(left), right(nullptr), value(nullptr), var_type(nullptr), lexeme("") {}

Node::Node(TypeNode type, int value)
    : type(type), left(nullptr), right(nullptr), value(new int(value)), var_type(nullptr), lexeme("") {}

Node::Node(TypeNode type, std::string lexeme)
    : type(type), left(nullptr), right(nullptr), value(nullptr), var_type(nullptr), lexeme(lexeme) {}

Node::Node(TypeNode type, Node *left, std::string lexeme)
    : type(type), left(left), right(nullptr), value(nullptr), var_type(nullptr), lexeme(lexeme) {}

Node::Node(TypeNode type, Node *left, int value, std::string lexeme)
    : type(type), left(left), right(nullptr), value(new int(value)), var_type(nullptr), lexeme(lexeme) {}

Node::Node(TypeNode type, int value, std::string lexeme)
    : type(type), left(nullptr), right(nullptr), value(new int(value)), var_type(nullptr), lexeme(lexeme) {std::cout << "use\n";}

Node:: Node(TypeNode type, Node *left, Node *right, VariableTypes var_type)
    : type(type), left(left), right(right), value(nullptr), var_type(new VariableTypes(var_type)), lexeme("") {}

Node:: Node(TypeNode type, Node *left, VariableTypes var_type)
    : type(type), left(left), right(nullptr), value(nullptr), var_type(new VariableTypes(var_type)), lexeme("") {}

Node:: Node(TypeNode type, Node *left, VariableTypes var_type, std::string lexeme)
    : type(type), left(left), right(nullptr), value(nullptr), var_type(new VariableTypes(var_type)), lexeme(lexeme) {}

Node:: Node(TypeNode type, VariableTypes var_type, std::string lexeme)
    : type(type), left(nullptr), right(nullptr), value(nullptr), var_type(new VariableTypes(var_type)), lexeme(lexeme) {}

Node::~Node() {
    //std::cout << "deleting children" << '\n';
    if(value != nullptr) delete value;
    if(var_type != nullptr) delete var_type;
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
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
    std::string tstr = typeToString();
    bool has_value = value != nullptr;
    bool has_type = var_type != nullptr;
    bool has_lexeme = lexeme != "";

    if (has_value && has_type && has_lexeme) return tstr + '(' + Variable::typeToString(*var_type) + ", " + std::to_string(*value) + ", " + lexeme + ')';
    if (has_value && has_type) return tstr + '(' + Variable::typeToString(*var_type) + ", " + std::to_string(*value) + ')';
    if (has_value && has_lexeme) return tstr + '(' + std::to_string(*value) + ", " + lexeme + ')';
    if (has_type && has_lexeme) return tstr + '(' + Variable::typeToString(*var_type) + ", " + lexeme + ')';
    if (has_value) return tstr + '(' +  std::to_string(*value) + ')';
    if (has_type) return tstr + '(' + Variable::typeToString(*var_type) + ')';
    if (has_lexeme) return tstr + '(' + lexeme + ')';

    return tstr;
}


std::string Node::typeToString() {
    switch (type) {
        case id: return "id";
        case number: return "number";
        case character: return "character";
        case const_string: return "const_string";
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
        case new_line: return "new_line";
        case var: return "var";
        case list_var: return "list_var";
        case list_decl_var: return "list_decl_var";
        case block: return "block";
        case func1: return "func1";
        case func2: return "func2";
        case param: return "param";
        case list_params: return "list_params";
        case func_call: return "func_call";
        case decl_func_var: return "decl_func_var";
        case prog: return "prog";
        case list_exp: return "list_exp";
    }

    return "";
}