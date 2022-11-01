#include <stdio.h>
#include "lib/text.hpp"
#include "tree.hpp"
#include "io.hpp"



/**
 * \brief Prints node and it's children
 * \param [in]  node   Node to start from
 * \param [out] stream Output file
 * \param [in]  shift  Output text offset
*/
void write_node(Node *node, FILE *stream, int shift);




int write_tree(Tree *tree, const char *filepath) {
    // ADD ASSERT HERE

    FILE *output = fopen(filepath, "w");

    // ADD ASSERT HERE

    write_node(tree -> root, output, 0);

    fclose(output);

    return 0;
}


void write_node(Node *node, FILE *stream, int shift) {
    if (!node) return;

    fprintf(stream, "%*s{", shift, "");

    fprintf(stream, " \"%i\" ", node -> data);

    if (node -> left) {
        fprintf(stream, "\n");
        write_node(node -> left, stream, shift + 4);

        if (node -> right)
            fprintf(stream, "\n");
        else
            fprintf(stream, "\n%*s{ \" \" }\n", shift + 4, "");
    }
    

    if (node -> right) {
        if (!node -> left)
            fprintf(stream, "\n%*s{ \" \" }\n", shift + 4, "");

        write_node(node -> right, stream, shift + 4);
        fprintf(stream, "\n");
    }

    if (node -> left || node -> right)
        fprintf(stream, "%*s}", shift, "");
    else
        fprintf(stream, "}");
}
