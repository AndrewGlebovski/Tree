#include <stdio.h>
#include "tree.hpp"


int main() {
    Tree tree = {};

    tree_constructor(&tree);

    tree.root -> left = create_node(1);
    tree.root -> right = create_node(2);
    tree.root -> right -> left = create_node(3);
    tree.root -> right -> right = create_node(4);

    print_tree(&tree);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
