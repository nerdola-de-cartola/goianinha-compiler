#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "node.hpp"

int main() {
    std::cout << "Testing Abstract Syntax Tree" << std::endl;
    
    auto root = new Node(add_op, 
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

    root->traverse_prev();

    delete root;
    
    return 0;
}
