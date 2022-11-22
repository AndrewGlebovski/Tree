#include <stdio.h>
#include "tree.hpp"
#include "io.hpp"
#include "dif.hpp"


int main() {
    Tree tree = {};

    printf("Enter function: ");
    read_tree(&tree);
    /* 
    double point = 0;
    printf("Enter point: ");
    scanf("%lg", &point);
    */
    int order = 0;
    printf("Enter derivative order: ");
    scanf("%i", &order);

    optimize(tree.root);

    printf("Function: ");
    print_tree(&tree);
    
    for(int i = 1; i <= order; i++) {
        Tree diff_tree = {diff(tree.root), 1};

        optimize(diff_tree.root);

        printf("%ith derivative: ", i);
        print_tree(&diff_tree);

        tree_destructor(&tree);
        tree = diff_tree;
    }
    
    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
