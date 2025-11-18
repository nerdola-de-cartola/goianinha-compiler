#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "node.hpp"

auto default_pos = yy::position(nullptr, 42, 49);
auto default_loc = yy::location(default_pos);

int main() {
    std::cout << "Testing Abstract Syntax Tree" << std::endl;
    
    auto root = new Node(add_op, 
        new Node(number, "8", default_loc),
        new Node(mul_op,
            new Node(number, "2", default_loc),
            new Node(or_op, 
                new Node(le_op,
                    new Node(number, "9", default_loc),
                    new Node(number, "6", default_loc),
                    default_loc
                ),
                new Node(number, "1", default_loc),
                default_loc
            ),
            default_loc
        ),
        default_loc
    );

    root->traverse_prev();

    delete root;
    
    return 0;
}
