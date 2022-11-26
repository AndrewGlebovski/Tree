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


Node *diff(const Node *node) {
    switch(node -> type) {
        case NODE_TYPES::TYPE_NUM:
            return create_num(0);
        case NODE_TYPES::TYPE_VAR:
            if (tolower(node -> value.var) == 'x') return create_num(1);
            else return create_num(0);
        case NODE_TYPES::TYPE_OP:
            switch (node -> value.op) {
                case OPERATORS::OP_ADD:
                    return Add(dL, dR);
                case OPERATORS::OP_SUB:
                    return Sub(dL, dR);
                case OPERATORS::OP_MUL:
                    return Add(Mul(dL, R), Mul(dR, L));
                case OPERATORS::OP_DIV:
                    return Div(Sub(Mul(dL, R), Mul(dR, L)), Mul(R, R));
                case OPERATORS::OP_POW:
                    if ((IS_TYPE(node -> left, VAR) || IS_TYPE(node -> left, OP)) && (IS_TYPE(node -> right, VAR) || IS_TYPE(node -> right, OP)))
                        return Mul(Pow(L, R), Add(Mul(dL, Ln(R)), Div(Mul(dR, L), R)));
                    else if (IS_TYPE(node -> left, VAR) || IS_TYPE(node -> left, OP))
                        return Mul(Mul(R, Pow(L, Sub(R, create_num(1)))), dL);
                    else
                        return Mul(Mul(Pow(L, R), Ln(L)), dR);
                case OPERATORS::OP_LOG:
                    return Div(dR, Mul(R, Ln(L)));
                case OPERATORS::OP_SIN:
                    return Mul(Cos(R), dR);
                case OPERATORS::OP_COS:
                    return Mul(Mul(create_num(-1), Sin(R)), dR);
                default: 
                    return nullptr;
            }
        default:
            return nullptr;
    }
}


void copy_node(Node *origin, Node *destination) {
    destination -> type = origin -> type;
    destination -> value = origin -> value;
    destination -> left = origin -> left;
    destination -> right = origin -> right;

    free(origin);
}




void optimize(Node *node) {
    if (node -> type != NODE_TYPES::TYPE_OP)
        return;

    if (node -> left) optimize(node -> left);
    if (node -> right) optimize(node -> right);

    if (IS_OP(SIN)) {
        if (node -> left) {FREE(node -> left);}

        if (IS_TYPE(node -> right, NUM)) {
            node -> type = NODE_TYPES::TYPE_NUM;
            node -> value.dbl = sin(node -> right -> value.dbl);
        }
        else
            return;
    }
    else if (IS_OP(COS)) {
        if (node -> left) {FREE(node -> left);}

        if (IS_TYPE(node -> right, NUM)) {
            node -> type = NODE_TYPES::TYPE_NUM;
            node -> value.dbl = cos(node -> right -> value.dbl);
        }
        else
            return;
    }
    else if (IS_TYPE(node -> left, NUM) && IS_TYPE(node -> right, NUM)) {
        node -> value.dbl = calc_value(node, 1.0);
        node -> type = TYPE_NUM;
        
        FREE(node -> left);
    }
    else if (IS_OP(POW) && IS_NUM(right, 0)) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = 1;

        FREE(node -> left);
    }
    else if (IS_OP(POW) && IS_NUM(right, 1)) {
        FREE(node -> right);
        copy_node(node -> left, node);
        return;
    }
    else if (IS_OP(MUL) && (IS_NUM(right, 0) || IS_NUM(left, 0))) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = 0;

        FREE(node -> left);
    }
    else if (IS_OP(MUL) && IS_NUM(right, 1)) {
        FREE(node -> right);
        copy_node(node -> left, node);
        return;
    }
    else if (IS_OP(MUL) && IS_NUM(left, 1)) {
        FREE(node -> left);
        copy_node(node -> right, node);
        return;
    }
    else if (IS_OP(ADD) && IS_NUM(right, 0)) {
        FREE(node -> right);
        copy_node(node -> left, node);
        return;
    }
    else if (IS_OP(ADD) && IS_NUM(left, 0)) {
        FREE(node -> left);
        copy_node(node -> right, node);
        return;
    }
    else if (IS_OP(SUB) && IS_NUM(right, 0)) {
        FREE(node -> right);
        copy_node(node -> left, node);
        return;
    }
    else if (IS_OP(DIV) && IS_NUM(right, 1)) {
        FREE(node -> right);
        copy_node(node -> left, node);
        return;
    }
    else if (IS_OP(DIV) && node -> left -> type == TYPE_VAR && node -> right -> type == TYPE_VAR) {
        if (node -> left -> value.var == node -> right -> value.var) {
            node -> type = NODE_TYPES::TYPE_NUM;
            node -> value.dbl = 1;

            FREE(node -> left);
        }
    }
    else {
        return;
    }

    FREE(node -> right);
}


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
