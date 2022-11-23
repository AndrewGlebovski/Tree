#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.hpp"
#include "io.hpp"




typedef struct {
    char *buffer = nullptr;
    int offset = 0;
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
 * \param [in] buffer Buffer to read from
 * \return Pointer to new node
*/
Node *read_node(Stream *stream);


/// Returns stream first symbol considering offset
char get_stream_char(Stream *stream);




int read_tree(Tree *tree) {
    Stream stream = {};

    stream.buffer = (char *) calloc(512, sizeof(char));

    fgets(stream.buffer, 512, stdin);

    stream.buffer[strlen(stream.buffer) - 1] = '\0';

    tree -> root = read_node(&stream);

    free(stream.buffer);

    return 0;
}


char get_stream_char(Stream *stream) {
    return *(stream -> buffer + stream -> offset);
}


Node *read_node(Stream *stream) {
    Node *node = (Node *) calloc(1, sizeof(Node));

    if (get_stream_char(stream) == '(') // first ( check
        stream -> offset++;
    else 
        return nullptr;

    if (get_stream_char(stream) == '(') { // operator expression
        node -> type = NODE_TYPES::TYPE_OP;

        node -> left = read_node(stream);

        if (!node -> left) return nullptr;

        node -> value.op = chr2op(get_stream_char(stream));

        stream -> offset++;

        node -> right = read_node(stream);

        if (!node -> right) return nullptr;

        if (node -> value.op == OP_ADD || node -> value.op == OP_MUL) {
            if (node -> right -> type == NODE_TYPES::TYPE_VAR) {
                Node *swap = node -> right;
                node -> right = node -> left;
                node -> left = swap;
            }
        }
    }

    else { // static expresssion
        if (isalpha(get_stream_char(stream))) {
            node -> type = NODE_TYPES::TYPE_VAR;

            node -> value.var = get_stream_char(stream);

            stream -> offset++; // skip variable
        }
        else {
            node -> type = NODE_TYPES::TYPE_NUM;

            int n = 0;
            sscanf(stream -> buffer + stream -> offset, "%lg%n", &node -> value.dbl, &n);

            stream -> offset += n; // skip number
        }
    }

    stream -> offset++; // closing )

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

    switch (node -> type) {
        case NODE_TYPES::TYPE_OP:
            fprintf(stream, " \"%i\" ", node -> value.op);
            break;
        case NODE_TYPES::TYPE_NUM:
            fprintf(stream, " \"%g\" ", node -> value.dbl);
            break;
        case NODE_TYPES::TYPE_VAR:
            fprintf(stream, " \"%c\" ", node -> value.var);
            break;
        default:
            break;
    }

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
