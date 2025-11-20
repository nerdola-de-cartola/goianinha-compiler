#ifndef NODE_HPP
#define NODE_HPP

#include <variable.hpp>
#include <string>
#include "location.hh"


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
    VariableTypes *var_type;
    std::string lexeme;
    yy::location &loc;

public:
    Node(TypeNode type, yy::location &loc);
    Node(TypeNode type, Node *left, yy::location &loc);
    Node(TypeNode type, std::string lexeme, yy::location &loc);
    Node(TypeNode type, Node *left, Node *right, yy::location &loc);
    Node(TypeNode type, Node *left, std::string lexeme, yy::location &loc);
    Node(TypeNode type, Node *left, VariableTypes var_type, yy::location &loc);
    Node(TypeNode type, VariableTypes var_type, std::string lexeme, yy::location &loc);
    Node(TypeNode type, Node *left, Node *right, VariableTypes var_type, yy::location &loc);
    Node(TypeNode type, Node *left, VariableTypes var_type, std::string lexeme, yy::location &loc);
    ~Node();

    void traverse_prev();
    void traverse_in();
    void traverse_pos();
    void printTree(std::string prefix = "", bool isLeft = true);
    std::string toString();
    std::string typeToString();
};

#endif // NODE_HPP
