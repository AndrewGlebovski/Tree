#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


/**
 * \brief Prints node and its children
 * \param [in]  node   Node to print
 * \param [out] stream Output stream
*/
void print_node(Node *node, FILE *stream);


/**
 * \brief Find node by its value
 * \param [in]  node  Search will start from this node
 * \param [in]  value Searching for this value
 * \return Actual pointer if node was found or nullptr
*/
Node *fast_search(Node *node, const char *value);


/**
 * \brief Find node by its value and store path to it
 * \param [in]  tree  Search will start from this node
 * \param [in]  value Searching for this value
 * \param [out] path  Contains path to node (path ends with nullptr)
 * \return Actual pointer if node was found or nullptr
*/
Node *full_search(Node *node, const char *value, Node *path[]);




int tree_constructor(Tree *tree, Node *root) {
    ASSERT(tree, "Constructor can't work with null pointer to tree!", INVALID_ARG);

    if (root) {
        tree -> root = root;
    }

    else {
        tree -> root = create_node(nullptr);
        
        ASSERT(tree -> root, "Failed to allocate tree root!", ALLOC_FAIL);
    }

    tree -> size = 1;

    return 0;
}


Node *create_node(const char *value, Node *left, Node *right) {
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

    free((char *) node -> data);

    free(node);
}


void print_tree(Tree *tree, FILE *stream) {
    // ADD ASSERT HERE
    
    print_node(tree -> root, stream);

    fputc('\n', stream);
}


void print_node(Node *node, FILE *stream) {
    if (!node) return;

    fprintf(stream, "( ");

    if (node -> left) print_node(node -> left, stream);

    fprintf(stream, "%s ", node -> data);

    if (node -> left) print_node(node -> right, stream);

    fprintf(stream, ") ");
}


Node *find_in_tree(Tree *tree, const char *value, Node *path[]) {
    // ADD ASSERT HERE

    if (path)
        return full_search(tree -> root, value, path);
    else
        return fast_search(tree -> root, value);
}


Node *fast_search(Node *node, const char *value) {
    if (!node) return nullptr;

    if (node -> data == value) return node;

    Node *result = nullptr;

    if (node -> left) {
        if ((result = fast_search(node -> left, value)))
            return result;
    }

    if (node -> right) {
        if ((result = fast_search(node -> right, value)))
            return result;
    }

    return nullptr;
}


Node *full_search(Node *node, const char *value, Node *path[]) {
    if (!node) return nullptr;

    *path = node;

    if (strcmp(node -> data, value) == 0) return node;

    Node *result = nullptr;

    if (node -> left) {
        if ((result = full_search(node -> left, value, path + 1)))
            return result;
    }

    if (node -> right) {
        if ((result = full_search(node -> right, value, path + 1)))
            return result;
    }

    *path = nullptr;

    return nullptr;
}
