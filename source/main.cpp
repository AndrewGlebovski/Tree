#include <stdio.h>
#include "tree.hpp"
#include "io.hpp"
#include "dif.hpp"


int main() {
    Tree tree = {};

    read_tree(&tree);
    
    print_tree(&tree);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
