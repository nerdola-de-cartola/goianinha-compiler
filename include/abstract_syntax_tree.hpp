#ifndef ABSTRACT_SYNTAX_TREE_HPP
#define ABSTRACT_SYNTAX_TREE_HPP

#include "node.hpp"

class AbstractSyntaxTree {
private:
    Node *root;

public:
    AbstractSyntaxTree();
    ~AbstractSyntaxTree();
};

#endif // ABSTRACT_SYNTAX_TREE_HPP
