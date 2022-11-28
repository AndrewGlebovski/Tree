#include <stdio.h>
#include "tree.hpp"
#include "io.hpp"
#include "dif.hpp"


int main() {
    Tree tree = {};

    printf("Enter function: ");
    read_tree(&tree);
    
    double point = 0;
    printf("Enter point: ");
    scanf("%lg", &point);
    
    int order = 0;
    printf("Enter derivative order: ");
    scanf("%i", &order);
    
    create_derivative_doc(&tree, order, point);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
