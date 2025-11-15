#ifndef NODE_HPP
#define NODE_HPP

#include <variable.hpp>
#include <string>

enum TypeNode {
    id,
    number,
    character,
    const_string,
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
    command,
    loop,
    if_cond,
    if_blocks,
    write_cmd,
    read_cmd,
    return_cmd,
    new_line,
    var,
    list_var,
    list_decl_var,
    block,
    func1,
    func2,
    param,
    list_params,
    func_call,
    decl_func_var,
    prog,
    list_exp,
};

class Node {
public:
    TypeNode type;
    Node *left;
    Node *right;
    int *value;
    VariableTypes *var_type;
    std::string lexeme;

public:
    Node(TypeNode type);
    Node(TypeNode type, Node *left, Node *right, int value);
    Node(TypeNode type, Node *left, Node *right, VariableTypes var_type);
    Node(TypeNode type, Node *left, VariableTypes var_type);
    Node(TypeNode type, Node *left, Node *right);
    Node(TypeNode type, Node *left);
    Node(TypeNode type, int value);
    Node(TypeNode type, std::string lexeme);
    Node(TypeNode type, Node *left, std::string lexeme);
    Node(TypeNode type, Node *left, int value, std::string lexeme);
    Node(TypeNode type, Node *left, VariableTypes var_type, std::string lexeme);
    Node(TypeNode type, int value, std::string lexeme);
    Node(TypeNode type, VariableTypes var_type, std::string lexeme);
    ~Node();

    void traverse_prev();
    void traverse_in();
    void traverse_pos();
    void printTree(std::string prefix = "", bool isLeft = true);
    std::string toString();
    std::string typeToString();
};

#endif // NODE_HPP
