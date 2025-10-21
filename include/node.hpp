#ifndef NODE_HPP
#define NODE_HPP

#include <string>

enum TypeNode {
    id,
    number,
    assign_op,
    negative_op,
    add_op,
    sub_op,
    mul_op,
    div_op,
    or_op,
    and_op,
    not_op,
    less_op,
    le_op,
    greater_op,
    ge_op,
    eq_op,
    dif_op,

};

class Node {
private:
    TypeNode type;
    Node *left;
    Node *right;
    int value;

public:
    Node(TypeNode type, Node *left, Node *right, int value);
    Node(TypeNode type, Node *left, Node *right);
    Node(TypeNode type, int value);
    ~Node();

    void traverse();
    std::string toString();
};

#endif // NODE_HPP
