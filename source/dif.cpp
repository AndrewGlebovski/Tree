#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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


/// Prints Taylor series to file in LaTex style
void print_taylor(FILE *file, int order, double *values, double point);


/// Prints tangent line to file in LaTex style 
void print_tangent(FILE *file, double k, double b, double point);


#define PRINT(...) fprintf(file, __VA_ARGS__)


#define DEF_GEN(op, create_node, ...)               \
    case OP_##op:                                   \
        result = create_node;                       \
        break;


Node *diff(const Node *node, FILE *file, const char *phrases[], int phrases_size) {
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
    
    if (file) {
        PRINT("%s\n", phrases[rand() % phrases_size]);

        PRINT("\\begin{center}\n%4s$", "");
        convert_to_latex(node, file);

        PRINT("\' = ");

        convert_to_latex(result, file);
        PRINT("$\n\\end{center}\n");
    }
    
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


void optimize(Node *node, FILE *file, const char *phrases[], int phrases_size) {
    if (node -> type != NODE_TYPES::TYPE_OP)
        return;

    if (node -> left) optimize(node -> left, file, phrases, phrases_size);
    if (node -> right) optimize(node -> right, file, phrases, phrases_size);

    if (file) {
        PRINT("%s\n", phrases[rand() % phrases_size]);
        
        PRINT("\\begin{center}\n%4s$", "");
        convert_to_latex(node, file);
    }

    switch (node -> value.op) {
        #include "gen.hpp"
        default:
            return;
    }

    if (file) {
        PRINT(" = ");
        convert_to_latex(node, file);
        PRINT("$\n\\end{center}\n");
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


void print_taylor(FILE *file, int order, double *values, double point) {
    NodeValue value;
    value.var = 'x';

    Tree func = {};
    Node *node = create_num(values[0]);

    double fact = 1.0;

    for(int i = 1; i <= order; i++) {
        node = Add(node, Mul(create_num(values[i] / fact), Pow(Sub(create_node(TYPE_VAR, value), create_num(point)), create_num(i))));

        func.root = node;

        fact *= i;
    }

    optimize(func.root, nullptr, nullptr, 0);

    print_function(func.root, file);

    tree_destructor(&func);
}


void print_tangent(FILE *file, double k, double b, double point) {
    NodeValue value;
    value.var = 'x';

    Tree func = {};
    func.root = Add(Mul(create_num(k), Sub(create_node(TYPE_VAR, value), create_num(point))), create_num(b));

    optimize(func.root, nullptr, nullptr, 0);

    print_function(func.root, file);

    tree_destructor(&func);
}


void create_derivative_doc(Tree *tree, int order, double point, int taylor_order, double min_value, double max_value) {
    #include "phrases.hpp"

    FILE *file = fopen("debug/main.tex", "w");

    srand((unsigned int) time(nullptr));

    PRINT("\\documentclass[12pt]{article}\n");
    PRINT("\\usepackage[utf8]{inputenc}\n");
    PRINT("\\usepackage[T2A]{fontenc}\n");
    PRINT("\\usepackage[russian]{babel}\n");
#ifdef FUNC_GRAPH
    PRINT("\\usepackage{graphics}\n");
#endif
    PRINT("\\setcounter{secnumdepth}{0}\n"); // отключает нумерацию секций

    PRINT("\\title{Сборник задач по математическому анализу, том 1, параграф %i, упражнение номер %i}\n", rand()%40, rand()%200);
    // PRINT("\\author{Тимофей Абишев, Б05-232}\n");

    PRINT("\\begin{document}\n");
    
    PRINT("\\maketitle\n");
    PRINT("\\tableofcontents\n");

    PRINT("\\section{Функция}\n");
    optimize(tree -> root, nullptr, nullptr, 0);
    print_function(tree -> root, file);

    Tree diff_tree = {tree -> root, 0}, free_queue = {};

    int max_order = (order > taylor_order)? order : taylor_order;

    double *values = (double *) calloc(max_order + 1, sizeof(double));

    values[0] = calc_value(tree -> root, point);

    for(int i = 1; i <= max_order; i++) {
        if (i < order) {
            PRINT("\\section{Взятие производной %i-го порядка}\n", i);
            PRINT("\\subsection{Дифференцирование}\n");
        }

        diff_tree.root = diff(diff_tree.root, file, diff_phrases, sizeof(diff_phrases) / sizeof(const char *));

        if (free_queue.root) tree_destructor(&free_queue);
        
        if (i < order) {
            PRINT("\\subsection{Оптимизация}\n");
            optimize(diff_tree.root, file, opti_phrases, sizeof(opti_phrases) / sizeof(const char *));

            PRINT("\\subsection{Производная %i-го порядка}\n", i);
            print_function(diff_tree.root, file);
        }

        values[i] = calc_value(diff_tree.root, point);

        free_queue = diff_tree;
    }

    if (free_queue.root) tree_destructor(&free_queue);

    PRINT("\\section{Касательная в точке %lg}\n", point);
    print_tangent(file, values[1], values[0], point);
    
    PRINT("\\section{Формула Тейлора, куда ж без нее}\n");
    print_taylor(file, taylor_order, values, point);
    
#ifdef FUNC_GRAPH
    create_func_graph(tree, "plot", min_value, max_value);

    PRINT("\\section{Прекраснейший график}\n");
    PRINT("\\begin{center}\n");
    PRINT("%4s\\includegraphics{plot.png}\n", "");
    PRINT("\\end{center}\n");
#endif
    
    PRINT("\\section{Источники}\n");
    PRINT("Это совершенно секретная информация, поэтому я скормил ее Полторашке\n");

    PRINT("\\end{document}\n");

    free(values);

    fclose(file);
}


void print_function(const Node *node, FILE *file) {
    PRINT("\\begin{center}\n%4s$y = ", "");
    convert_to_latex(node, file);
    PRINT("$\n\\end{center}\n");
}


#define PRINT_IN_BRACKETS(child)                               \
fputc('(', file);                                              \
convert_to_latex(node -> child, file);                         \
fputc(')', file)


#define PRINT_TO_LATEX(child)                                  \
if (IS_TYPE(node -> child, OP)) { PRINT_IN_BRACKETS(child); }  \
else {convert_to_latex(node -> child, file);}


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
            switch (node -> value.op) {
                #include "gen.hpp"
                default:
                    convert_to_latex(node -> left, file);
                    PRINT("%s", op2str(node -> value.op));
                    convert_to_latex(node -> right, file);
                    break;
            }
        default: break;
    }

    fputc('}', file);
}

#undef DEF_GEN


#ifdef FUNC_GRAPH

void create_func_graph(Tree *tree, const char *filename, double min_value, double max_value) {
    char *cmd = (char *) calloc(2048, sizeof(char));
    int offset = 0;

    sprintf(cmd, "python source/plot.py %s %lg %lg %lg %n", filename, 0.02, min_value, max_value, &offset);

    for (double i = min_value + 0.01; i < max_value; i += 0.02) {
        int n = 0;
        sprintf(cmd + offset, "%.3lg %n", calc_value(tree -> root, i), &n);
        offset += n;
    }

    system(cmd);
}

#endif
