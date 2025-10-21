#include "node.hpp"
#include <iostream>


Node::Node(TypeNode type, Node *left, Node *right, int value)
    : type(type), left(left), right(right), value(value) {}

Node::Node(TypeNode type, Node *left, Node *right)
    : type(type), left(left), right(right) {}

Node::Node(TypeNode type, int value)
    : type(type), left(nullptr), right(nullptr), value(value) {}

Node::~Node() {
    delete left;
    delete right;
}


void Node::traverse() {
    
    if (left != nullptr) {
        left->traverse();
    }
    
    if (right != nullptr) {
        right->traverse();
    }
    
    std::cout << toString() << '\n';

    return;
}

std::string Node::toString() {

    switch (type) {
        case assign_op:
            return "=";

        case negative_op:
            return "-";

        case add_op:
            return "+";

        case sub_op:
            return "-";

        case mul_op:
            return "*";

        case div_op:
            return "/";

        case or_op:
            return "||";

        case and_op:
            return "&&";

        case not_op:
            return "!";

        case less_op:
            return "<";

        case le_op:
            return "<=";

        case greater_op:
            return ">";

        case ge_op:
            return ">=";

        case eq_op:
            return "==";

        case dif_op:
            return "!=";

        default:
            return std::to_string(value);
    }

    return "";
}