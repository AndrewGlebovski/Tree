#include <stdio.h>
#include "tree.hpp"
#include "io.hpp"


int main() {
    Tree tree = {};

    read_tree(&tree, "debug/tree.txt");
    /*
    tree_constructor(&tree);
    
    tree.root -> left = create_node(1);
    tree.root -> right = create_node(2);
    tree.root -> right -> left = create_node(3);
    tree.root -> right -> right = create_node(4);
    tree.root -> right -> left -> left = create_node(5);
    tree.root -> right -> left -> left -> right = create_node(6);
    */
    Node *buffer[10] = {0};

    printf("%i\n", find_in_tree(&tree, 6, buffer) -> data);

    for(int i = 0; buffer[i] != nullptr; i++)
        printf("%i ", buffer[i] -> data);
    
    putchar('\n');

    write_tree(&tree, "debug/new_tree.txt");

    print_tree(&tree);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
