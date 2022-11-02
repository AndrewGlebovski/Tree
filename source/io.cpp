#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
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


/**
 * \brief Creates node and reads its data from buffer 
 * \param [in] buffer Buffer to read from
 * \return Pointer to new node
*/
Node *read_node(const char *buffer);




int read_tree(Tree *tree, const char *filepath) {
    // ADD ASSERT HERE

    int input = open(filepath, O_RDONLY);

    // ADD ASSERT HERE

    char *buffer = nullptr;

    read_in_buffer(input, &buffer, get_file_size(input));

    replace_in_buffer(buffer, '\n', ' ');

    tree -> root = read_node(buffer);

    free(buffer);

    return 0;
}


Node *read_node(const char *buffer) {
    static int offset = 0;

    String token = get_token(buffer + offset, "{\"}", ""); // {

    Node *node = create_node(0);

    token = get_token(token.str + token.len, "{\"}", ""); // "

    token = get_token(token.str + token.len, "{\"}", "");

    if (str_to_int(&token, &node -> data))
        node = nullptr;

    token = get_token(token.str + token.len, "{\"}", ""); // "

    String bracket = get_token(token.str + token.len, "{\"}", "");

    offset = (int)(bracket.str - buffer) + bracket.len;

    if (strncmp(bracket.str, "{", bracket.len) == 0) {
        offset -= bracket.len;
        
        node -> left = read_node(buffer);
        
        node -> right = read_node(buffer);
    }

    return node;
}



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
