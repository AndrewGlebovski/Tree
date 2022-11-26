#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "tree.hpp"
#include "dif.hpp"
#include "dsl.hpp"


///Copies origin parameters in destination and then destroys the origin
void copy_node(Node *origin, Node *destination);


/// Prints epxression with center alignment in LaTex style to file
void print_function(const Node *node, FILE *file);


/// Prints epxression in LaTex style to output file
void convert_to_latex(const Node *node, FILE *file);


#define PRINT(...) fprintf(file, __VA_ARGS__)


#define DEF_GEN(op, create_node, ...)               \
    case OP_##op:                                   \
        result = create_node;                       \
        break;


Node *diff(const Node *node, FILE *file) {
    Node *result = nullptr;

    switch(node -> type) {
        case NODE_TYPES::TYPE_NUM:
            result = create_num(0);
            break;
        case NODE_TYPES::TYPE_VAR:
            if (tolower(node -> value.var) == 'x') result = create_num(1);
            else result = create_num(0);
            break;
        case NODE_TYPES::TYPE_OP:
            switch (node -> value.op) {
                #include "gen.hpp"
                default: 
                    return nullptr;
            }
            break;
        default:
            return nullptr;
    }

    PRINT("Кому неочевидно, что это так, пусть первый кинет в меня камень\n");

    PRINT("\\begin{center}\n%4s$", "");
    convert_to_latex(node, file);

    PRINT("\' = ");

    convert_to_latex(result, file);
    PRINT("$\n\\end{center}\n");

    return result;
}

#undef DEF_GEN


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


void optimize(Node *node, FILE *file) {
    if (node -> type != NODE_TYPES::TYPE_OP)
        return;

    if (node -> left) optimize(node -> left, file);
    if (node -> right) optimize(node -> right, file);

    PRINT("Шлеп\n");
    PRINT("\\begin{center}\n%4s$", "");
    convert_to_latex(node, file);

    switch (node -> value.op) {
        #include "gen.hpp"
        default:
            return;
    }

    PRINT(" = ");
    convert_to_latex(node, file);
    PRINT("$\n\\end{center}\n");
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


void create_derivative_doc(Tree *tree, int order, double point) {
    FILE *file = fopen("debug/main.tex", "w");

    PRINT("\\documentclass[12pt]{article}\n");
    PRINT("\\usepackage[utf8]{inputenc}\n");
    PRINT("\\usepackage[T2A]{fontenc}\n");
    PRINT("\\usepackage[russian]{babel}\n");
    PRINT("\\setcounter{secnumdepth}{0}\n"); // отключает нумерацию секций

    PRINT("\\title{Докторская диссертация}\n");
    PRINT("\\author{Тимофей Абишев, Б05-232}\n");

    PRINT("\\begin{document}\n");
    
    PRINT("\\maketitle\n");
    PRINT("\\tableofcontents\n");

    PRINT("\\section{Функция}\n");
    print_function(tree -> root, file);
    
    PRINT("\\subsection{Оптимизация}\n");
    optimize(tree -> root, file);

    Tree diff_tree = {tree -> root, 0}, free_queue = {};

    double *values = (double *) calloc(order + 1, sizeof(int));

    values[0] = calc_value(tree -> root, point);

    for(int i = 1; i <= order; i++) {
        PRINT("\\section{Взятие производной %i-го порядка}\n", i);

        PRINT("\\subsection{Дифференцирование}\n");
        diff_tree.root = diff(diff_tree.root, file);

        if (free_queue.root) tree_destructor(&free_queue);

        PRINT("\\subsection{Оптимизация}\n");
        optimize(diff_tree.root, file);

        PRINT("\\subsection{Производная %i-го порядка}\n", i);
        print_function(diff_tree.root, file);

        values[i] = calc_value(diff_tree.root, point);

        free_queue = diff_tree;
    }

    tree_destructor(&free_queue);

    PRINT("\\section{Касательная в точке %lg}\n", point);
    PRINT("\\begin{center}\n%4s$", "");
    PRINT("y = %lg(x - %lg) + %lg", values[1], point, values[0]);
    PRINT("$\n\\end{center}\n");
    
    PRINT("\\section{Формула Тейлора, куда же без нее}\n");
    PRINT("\\begin{center}\n%4s$y = ", "");
    for(int i = 0; i <= order; i++)
        PRINT("\\frac{%lg}{%i!}(x - %lg)^%i + ", values[i], i, point, i);
    PRINT("o((x - %lg)^%i)$\n\\end{center}\n", point, order);

    PRINT("\\end{document}\n");

    free(values);

    fclose(file);
}


void print_function(const Node *node, FILE *file) {
    PRINT("\\begin{center}\n%4s$y = ", "");
    convert_to_latex(node, file);
    PRINT("$\n\\end{center}\n");
}


#define DEF_GEN(op, create_node, opti_node, calc_node, ...)    \
    case OP_##op:                                              \
        __VA_ARGS__;                                           \
        break;


void convert_to_latex(const Node *node, FILE *file) {
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
