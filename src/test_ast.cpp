#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "abstract_syntax_tree.hpp"
#include "node.hpp"

int main() {
    std::cout << "Testing Abstract Syntax Tree" << std::endl;
    
    AbstractSyntaxTree ast = AbstractSyntaxTree();
    auto a = new Node(add_op, 
        new Node(number, 8),
        new Node(mul_op,
            new Node(number, 2),
            new Node(or_op, 
                new Node(le_op,
                    new Node(number, 9),
                    new Node(number, 6)
                ),
                new Node(number, 1)
            )
        )
    );

    a->traverse_prev();

    delete a;
    
    return 0;
}
