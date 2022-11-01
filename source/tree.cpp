#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"


#define ASSERT(condition, message, error)                                    \
do {                                                                         \
    if (!(condition)) {                                                      \
        printf("%s(%i) in %s\n[%i] %s\n",                                    \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, error, message);        \
        return error;                                                        \
    }                                                                        \
} while(0)


/**
 * \brief Free node and its children
 * \param [in] node Node pointer
*/
void free_node(Node *node);




int tree_constructor(Tree *tree, Node *root) {
    ASSERT(tree, "Constructor can't work with null pointer to tree!", INVALID_ARG);

    if (root) {
        tree -> root = root;
    }

    else {
        tree -> root = create_node(0);
        
        ASSERT(tree -> root, "Failed to allocate tree root!", ALLOC_FAIL);
    }

    tree -> size = 1;

    return 0;
}


Node *create_node(int value, Node *left, Node *right) {
    Node *node = (Node *) calloc(1, sizeof(Node));

    if (node) {
        node -> data = value;
        node -> left = left;
        node -> right = right;
    }

    return node;
}


int tree_destructor(Tree *tree) {
    ASSERT(tree, "Constructor can't work with null pointer to tree!", INVALID_ARG);
    ASSERT(tree -> root, "Tree has null root!", INVALID_ARG);

    free_node(tree -> root);

    tree -> root = nullptr;
    tree -> size = 0;

    return 0;
}


void free_node(Node *node) {
    if (!node) return;

    if (node -> left) {
        free_node(node -> left);
        node -> left = nullptr;
    }

    if (node -> right) {
        free_node(node -> right);
        node -> right = nullptr;
    }

    free(node);
}
