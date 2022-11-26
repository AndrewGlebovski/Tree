#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "tree.hpp"
#include "dif.hpp"
#include "dsl.hpp"


void copy_node(Node *origin, Node *destination);


void print_function(Node *node, FILE *file);


void convert_to_latex(Node *node, FILE *file);


#define TEMPLATE(num_child, origin_child, num)      \
if (IS_NUM(num_child, num)) {                       \
    FREE(node -> num_child);                        \
    copy_node(node -> origin_child, node);          \
}

#define CONST_CHECK()                                               \
if (IS_TYPE(node -> left, NUM) && IS_TYPE(node -> right, NUM)) {    \
    node -> value.dbl = calc_value(node, 1.0);                      \
    node -> type = TYPE_NUM;                                        \
    FREE(node -> left);                                             \
    FREE(node -> right);                                            \
}

#define DEF_GEN(op, create_node, ...)               \
    case OP_##op:                                   \
        return create_node;


Node *diff(const Node *node) {
    switch(node -> type) {
        case NODE_TYPES::TYPE_NUM:
            return create_num(0);
        case NODE_TYPES::TYPE_VAR:
            if (tolower(node -> value.var) == 'x') return create_num(1);
            else return create_num(0);
        case NODE_TYPES::TYPE_OP:
            switch (node -> value.op) {
                #include "gen.hpp"
                default: 
                    return nullptr;
            }
        default:
            return nullptr;
    }
}

#undef DEF_GEN


void copy_node(Node *origin, Node *destination) {
    destination -> type = origin -> type;
    destination -> value = origin -> value;
    destination -> left = origin -> left;
    destination -> right = origin -> right;

    free(origin);
}

#define DEF_GEN(op, create_node, opti_node, ...)    \
case OP_##op:                                       \
    opti_node;                                      \
    break;


void optimize(Node *node) {
    if (node -> type != NODE_TYPES::TYPE_OP)
        return;

    if (node -> left) optimize(node -> left);
    if (node -> right) optimize(node -> right);

    switch (node -> value.op) {
        #include "gen.hpp"
        default:
            return;
    }
}

#undef DEF_GEN


#define DEF_GEN(op, create_node, opti_node, calc_node, ...)    \
    case OP_##op:                                              \
        return calc_node;


double calc_value(const Node *node, double x) {
    switch (node -> type) {
        case TYPE_NUM: return node -> value.dbl;
        case TYPE_VAR: return x;
        case TYPE_OP:
            switch (node -> value.op) {
                #include "gen.hpp"
                default: 
                    return 0.0;
            }
        default: return 0.0;
    }
}

#undef DEF_GEN


#define PRINT(...) fprintf(file, __VA_ARGS__)


void create_derivative_doc(Tree *tree, int order, double point) {
    FILE *file = fopen("debug/main.tex", "w");

    PRINT("\\documentclass[12pt]{article}\n");
    PRINT("\\usepackage[utf8]{inputenc}\n");
    PRINT("\\usepackage[T2A]{fontenc}\n");
    PRINT("\\usepackage[russian]{babel}\n");

    PRINT("\\begin{document}\n");

    PRINT("Докторская диссертация\\\\\n");

    PRINT("Функция:\n");
    print_function(tree -> root, file);
    
    optimize(tree -> root);

    PRINT("Оптимизированная функция\n");
    print_function(tree -> root, file);

    Tree diff_tree = {tree -> root, 0}, free_queue = {};

    double *values = (double *) calloc(order + 1, sizeof(int));

    values[0] = calc_value(tree -> root, point);

    for(int i = 1; i <= order; i++) {
        PRINT("Дифференцируем\n");
        diff_tree.root = diff(diff_tree.root);
        print_function(diff_tree.root, file);

        if (free_queue.root) tree_destructor(&free_queue);

        PRINT("Оптимизируем\\\\\n");
        optimize(diff_tree.root);

        PRINT("А вот и производная %i-го порядка:\n", i);
        print_function(diff_tree.root, file);

        values[i] = calc_value(diff_tree.root, point);

        free_queue = diff_tree;
    }

    tree_destructor(&free_queue);

    PRINT("А вот и ее касательная в точке %lg:\n", point);
    PRINT("\\begin{center}\n%4s$", "");
    PRINT("y = %lg(x - %lg) + %lg", values[1], point, values[0]);
    PRINT("$\n\\end{center}\n");
    
    PRINT("Формула Тейлора, куда же без нее:\\\\\n");
    PRINT("\\begin{center}\n%4s$y = ", "");
    for(int i = 0; i <= order; i++)
        PRINT("\\frac{%lg}{%i!}(x - %lg)^%i + ", values[i], i, point, i);
    PRINT("o((x - %lg)^%i)$\n\\end{center}\n", point, order);

    PRINT("\\end{document}\n");

    free(values);

    fclose(file);
}


void print_function(Node *node, FILE *file) {
    PRINT("\\begin{center}\n%4s$y = ", "");
    convert_to_latex(node, file);
    PRINT("$\n\\end{center}\n");
}


#define DEF_GEN(op, create_node, opti_node, calc_node, ...)    \
    case OP_##op:                                              \
        __VA_ARGS__;                                           \
        break;


void convert_to_latex(Node *node, FILE *file) {
    if (!node) return;

    fputc('{', file);

    switch (node -> type) {
        case TYPE_NUM:
            if (is_equal(node -> value.dbl, _PI))
                PRINT("\\Pi");
            else if (is_equal(node -> value.dbl, _EXP))
                PRINT("e");
            else
                PRINT("%lg", node -> value.dbl);
            break;
        case TYPE_VAR: PRINT("%c", node -> value.var); break;
        case TYPE_OP: 
            fputc('(', file);
            switch (node -> value.op) {
                #include "gen.hpp"
                default:
                    convert_to_latex(node -> left, file);
                    PRINT("%s", op2str(node -> value.op));
                    convert_to_latex(node -> right, file);
                    break;
            }
            fputc(')', file);
        default: break;
    }

    fputc('}', file);
}

#undef DEF_GEN
