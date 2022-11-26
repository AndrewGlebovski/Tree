#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "tree.hpp"
#include "io.hpp"
#include "dsl.hpp"




Node *getG(const char *str);
Node *getE();         ///< E::=T{[+-]T}*
Node *getT();         ///< T::=D{[*/]D}*
Node *getD();         ///< D::=U{'^'U}*
Node *getU();         ///< U::=['-''sin''cos''ln']P|P
Node *getP();         ///< P::='('E')'|N
Node *getV();
Node *getN();         ///< N::=['0'-'9']


const char *s = nullptr;


/**
 * \brief Prints node and it's children
 * \param [in]  node   Node to start from
 * \param [out] stream Output file
 * \param [in]  shift  Output text offset
*/
void write_node(Node *node, FILE *stream, int shift);




int read_tree(Tree *tree) {
    char *buffer = (char *) calloc(512, sizeof(char));

    fgets(buffer, 512, stdin);

    buffer[strlen(buffer) - 1] = '\0';

    tree -> root = getG(buffer);

    free(buffer);

    return 0;
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


Node *getG(const char *str) {
    s = str;
    
    Node *value = getE();

    assert(*s == '\0' && "No \0 at the end of expression!");

    return value;
}


Node *getE() {
    Node *value = getT();

    if (*s == '+' || *s == '-') {
        char op = *s; 

        s++;
        Node *tmp = getE();

        if (op == '+') return Add(value, tmp);
        else return Sub(value, tmp);
    }
    else
        return value;
}


Node *getT() {
    Node *value = getD();

    if (*s == '*' || *s == '/') {
        char op = *s; 

        s++;
        Node *tmp = getT();

        if (op == '*') return Mul(value, tmp);
        else return Div(value, tmp);
    }
    else
        return value;
}


Node *getD() {
    Node *value = getU();

    if (*s == '^') {
        s++;
        Node *tmp = getD();

        return Pow(value, tmp);
    }

    return value;
}


Node *getU() {
    if (strncmp(s, "sin", 3) == 0) {
        s+=3;
        return Sin(getP());
    }
    else if (strncmp(s, "cos", 3) == 0) {
        s+=3;
        return Cos(getP());
    }
    else if (strncmp(s, "ln", 2) == 0) {
        s+=2;
        return Ln(getP());
    }
    else return getP();
}


Node *getP() {
    Node *value = {};

    if (*s == '(') {
        s++;
        value = getE();

        assert(*s == ')' && "No closing bracket!");

        s++;
    }
    else if ((value = getV())) ;
    else value = getN();
    
    return value;
}


Node *getV() {
    NodeValue value = {};
    
    if (isalpha(*s)) {
        value.var = *s;
        s++;
    }
    else
        return nullptr;

    return create_node(TYPE_VAR, value);
}


Node *getN() {
    Node *value = create_num(0.0);

    int read = 0;
    sscanf(s, "%lg%n", &value -> value.dbl, &read);

    s += read;

    assert(read && "No number found!");

    return value;
}
