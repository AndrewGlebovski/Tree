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

#define CONST_CHECK()                                                   \
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


double calc_value(const Node *node, double x) {
    switch (node -> type) {
        case TYPE_NUM: return node -> value.dbl;
        case TYPE_VAR: return x;
        case TYPE_OP:
            switch (node -> value.op) {
                case OPERATORS::OP_ADD: return calc_value(node -> left, x) + calc_value(node -> right, x);
                case OPERATORS::OP_SUB: return calc_value(node -> left, x) - calc_value(node -> right, x);
                case OPERATORS::OP_MUL: return calc_value(node -> left, x) * calc_value(node -> right, x);
                case OPERATORS::OP_DIV: return calc_value(node -> left, x) / calc_value(node -> right, x);
                case OPERATORS::OP_POW: return pow(calc_value(node -> left, x), calc_value(node -> right, x));
                case OPERATORS::OP_LOG: return log(calc_value(node -> right, x)) / log(calc_value(node -> left, x));
                case OPERATORS::OP_SIN: return sin(calc_value(node -> right, x));
                case OPERATORS::OP_COS: return cos(calc_value(node -> right, x));
                default: return 0.0;
            }
        default: return 0.0;
    }
}


void create_derivative_doc(Tree *tree, int order, double point) {
    FILE *file = fopen("debug/main.tex", "w");

    fprintf(file, "Сейчас я вам покажу производную этой достаточно сложной функциии\n");

    fprintf(file, "А вот и сама функция:\n");
    print_function(tree -> root, file);
    
    Tree diff_tree = {diff(tree -> root), 0};
    optimize(diff_tree.root);

    fprintf(file, "А вот и ее первая производная:\n");
    print_function(diff_tree.root, file);

    fprintf(file, "А вот и ее касательная в точке %lg:\n", point);
    fprintf(file, "\\begin{center}\n%4s$", "");
    fprintf(file, "y_{\\text{кас}} = %lg(x - %lg) + %lg", calc_value(diff_tree.root, point), point, calc_value(tree -> root, point));
    fprintf(file, "$\n\\end{center}\n");

    tree_destructor(&diff_tree);

    fclose(file);
}


void print_function(Node *node, FILE *file) {
    fprintf(file, "\\begin{center}\n%4s$", "");
    convert_to_latex(node, file);
    fprintf(file, "$\n\\end{center}\n");
}


void convert_to_latex(Node *node, FILE *file) {
    if (!node) return;

    fputc('{', file);

    switch (node -> type) {
        case TYPE_NUM:
            if (is_equal(node -> value.dbl, _PI))
                fprintf(file, "\\Pi");
            else if (is_equal(node -> value.dbl, _EXP))
                fprintf(file, "e");
            else
                fprintf(file, "%lg", node -> value.dbl);
            break;
        case TYPE_VAR: fprintf(file, "%c", node -> value.var); break;
        case TYPE_OP: 
            fputc('(', file);
            switch (node -> value.op) {
                case OPERATORS::OP_LOG:
                    fprintf(file, "log_");
                    convert_to_latex(node -> left, file);
                    fprintf(file, "^");
                    convert_to_latex(node -> right, file);
                    break;
                case OPERATORS::OP_SIN:
                    fprintf(file, "sin");
                    convert_to_latex(node -> right, file);
                    break;
                case OPERATORS::OP_COS:
                    fprintf(file, "cos");
                    convert_to_latex(node -> left, file);
                    break;
                case OPERATORS::OP_MUL:
                    convert_to_latex(node -> left, file);
                    // fprintf(file, "\\dot");
                    convert_to_latex(node -> right, file);
                    break;
                case OPERATORS::OP_DIV:
                    fprintf(file, "\\frac");
                    convert_to_latex(node -> left, file);
                    convert_to_latex(node -> right, file);
                    break;
                default:
                    convert_to_latex(node -> left, file);
                    fprintf(file, "%s", op2str(node -> value.op));
                    convert_to_latex(node -> right, file);
                    break;
            }
            fputc(')', file);
        default: break;
    }

    fputc('}', file);
}
