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
Node *fast_search(Node *node, int value);


/**
 * \brief Find node by its value and store path to it
 * \param [in]  tree  Search will start from this node
 * \param [in]  value Searching for this value
 * \param [out] path  Contains path to node (path ends with nullptr)
 * \return Actual pointer if node was found or nullptr
*/
Node *full_search(Node *node, int value, Node *path[]);




int tree_constructor(Tree *tree, Node *root) {
    ASSERT(tree, "Constructor can't work with null pointer to tree!", INVALID_ARG);

    if (root) {
        tree -> root = root;
        tree -> size = 1;
    }

    return 0;
}


Node *create_node(int type, NodeValue value, Node *left, Node *right) {
    Node *node = (Node *) calloc(1, sizeof(Node));

    if (node) {
        node -> type = type;
        node -> value = value;
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


void print_tree(Tree *tree, FILE *stream) {
    // ADD ASSERT HERE
    
    print_node(tree -> root, stream);

    fputc('\n', stream);
}


void print_node(Node *node, FILE *stream) {
    if (!node) return;

    fprintf(stream, "( ");

    if (node -> left) print_node(node -> left, stream);

    switch (node -> type) {
        case NODE_TYPES::OP:
            fprintf(stream, "%s ", op2str(node -> value.op));
            break;
        case NODE_TYPES::NUM:
            fprintf(stream, "%g ", node -> value.dbl);
            break;
        case NODE_TYPES::VAR:
            fprintf(stream, "%c ", node -> value.var);
            break;
        default:
            fprintf(stream, "@");
    }

    if (node -> left) print_node(node -> right, stream);

    fprintf(stream, ") ");
}


const char *op2str(int op) {
    switch(op) {
        case OPERATORS::OP_ADD: return "+";
        case OPERATORS::OP_SUB: return "-";
        case OPERATORS::OP_MUL: return "*";
        case OPERATORS::OP_DIV: return "/";
        default: return "#";
    }
}


int chr2op(char op) {
    switch(op) {
        case '+': return OPERATORS::OP_ADD;
        case '-': return OPERATORS::OP_SUB;
        case '*': return OPERATORS::OP_MUL;
        case '/': return OPERATORS::OP_DIV;
        default: return OP_ERR;
    }
}
