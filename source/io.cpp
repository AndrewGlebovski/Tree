#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "tree.hpp"
#include "io.hpp"
#include "dsl.hpp"




Node *getG(const char *str);        ///< G::=E'\0'
Node *getE(const char **s);         ///< E::=T{[+-]T}*
Node *getT(const char **s);         ///< T::=D{[*/]D}*
Node *getD(const char **s);         ///< D::=U{'^'U}*
Node *getU(const char **s);         ///< U::=['-''sin''cos''ln']P|P
Node *getP(const char **s);         ///< P::='('E')'|V|N
Node *getV(const char **s);         ///< V::=['a'-'z''A'-'Z']
Node *getN(const char **s);         ///< N::=['0'-'9']+


/**
 * \brief Prints node and it's children
 * \param [in]  node   Node to start from
 * \param [out] stream Output file
 * \param [in]  shift  Output text offset
*/
void write_node(Node *node, FILE *stream, int shift);


/**
 * \brief Removes all spaces from buffer
 * \param [out] buffer Buffer to remove from
*/
void clear_spaces(char *buffer);



int read_tree(Tree *tree, char *buffer) {
    clear_spaces(buffer);

    tree -> root = getG(buffer);

    return 0;
}


void clear_spaces(char *buffer) {
    int read = 0, write = 0;

    for (; buffer[read] != '\0'; read++) {
        if (!isspace(buffer[read]))
            buffer[write++] = buffer[read];
    }

    buffer[write] = '\0';
}


int write_tree(Tree *tree, const char *filepath) {
    FILE *output = fopen(filepath, "w");

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
    const char *s = str;
    
    Node *value = getE(&s);

    assert(*s == '\0' && "No \0 at the end of expression!");

    return value;
}


Node *getE(const char **s) {
    Node *value = getT(s);

    if (**s == '+' || **s == '-') {
        char op = **s; 

        *s += 1;
        Node *tmp = getE(s);

        if (op == '+') return Add(value, tmp);
        else return Sub(value, tmp);
    }
    else
        return value;
}


Node *getT(const char **s) {
    Node *value = getD(s);

    if (**s == '*' || **s == '/') {
        char op = **s; 

        *s += 1;
        Node *tmp = getT(s);

        if (op == '*') return Mul(value, tmp);
        else return Div(value, tmp);
    }
    else
        return value;
}


Node *getD(const char **s) {
    Node *value = getU(s);

    if (**s == '^') {
        *s += 1;
        Node *tmp = getD(s);

        return Pow(value, tmp);
    }

    return value;
}


Node *getU(const char **s) {
    if (strncmp(*s, "sin", 3) == 0) {
        *s += 3;
        return Sin(getP(s));
    }
    else if (strncmp(*s, "cos", 3) == 0) {
        *s += 3;
        return Cos(getP(s));
    }
    else if (strncmp(*s, "ln", 2) == 0) {
        *s += 2;
        return Ln(getP(s));
    }
    else return getP(s);
}


Node *getP(const char **s) {
    Node *value = {};

    if (**s == '(') {
        *s += 1;
        value = getE(s);

        assert(**s == ')' && "No closing bracket!");

        *s += 1;
    }
    else if ((value = getV(s))) ;
    else value = getN(s);
    
    return value;
}


Node *getV(const char **s) {
    NodeValue value = {};
    
    if (isalpha(**s)) {
        value.var = **s;
        *s += 1;
    }
    else
        return nullptr;

    return create_node(TYPE_VAR, value);
}


Node *getN(const char **s) {
    Node *value = create_num(0.0);

    int read = 0;
    sscanf(*s, "%lg%n", &value -> value.dbl, &read);

    *s += read;

    assert(read && "No number found!");

    return value;
}
