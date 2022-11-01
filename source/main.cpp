#include <stdio.h>
#include "tree.hpp"


int main() {
    Tree tree = {};

    tree_constructor(&tree);

    tree.root -> left = create_node(1);
    tree.root -> right = create_node(2);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
