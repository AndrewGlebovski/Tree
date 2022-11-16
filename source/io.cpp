#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "lib/text.hpp"
#include "tree.hpp"
#include "io.hpp"


/// Stream implementation for string
typedef struct {
    char *buffer = nullptr;     ///< Char array
    int offset = 0;             ///< Offset from buffer first element
} Stream;


/**
 * \brief Prints node and it's children
 * \param [in]  node   Node to start from
 * \param [out] stream Output file
 * \param [in]  shift  Output text offset
*/
void write_node(Node *node, FILE *stream, int shift);


/**
 * \brief Creates node and reads its data from buffer 
 * \param [in] stream Buffer to read from and its offset
 * \return Pointer to new node
*/
Node *read_node(Stream *stream);




int read_tree(Tree *tree, const char *filepath) {
    // ADD ASSERT HERE

    int input = open(filepath, O_RDONLY);

    // ADD ASSERT HERE

    Stream stream = {};

    read_in_buffer(input, &stream.buffer, get_file_size(input));

    replace_in_buffer(stream.buffer, '\n', ' ');

    tree -> root = read_node(&stream);

    free(stream.buffer);

    return 0;
}


Node *read_node(Stream *stream) {
    Node *node = create_node(0);

    const char *first_quote  = strchr(stream -> buffer + stream -> offset, '"');
    const char *second_quote = strchr(first_quote + 1, '"');

    char *data = (char *) calloc(100, sizeof(char));
    
    *(stream -> buffer + (second_quote - stream -> buffer)) = '\0';

    strcpy(data, first_quote + 1);

    node -> data = data;

    String bracket = get_token(second_quote + 1, "{\"}", "");

    stream -> offset = (int)(bracket.str - stream -> buffer) + bracket.len;

    if (strncmp(bracket.str, "{", bracket.len) == 0) {
        stream -> offset -= bracket.len;
        
        node -> left = read_node(stream);
        
        node -> right = read_node(stream);
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

    fprintf(stream, " \"%s\" ", node -> data);

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
