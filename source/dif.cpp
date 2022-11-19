#include <stdio.h>
#include <math.h>
#include "tree.hpp"
#include "dif.hpp"


/**
 * \brief Shortcut for creating numeric type node
 * \param [in] num Node will be init with this value
 * \return New node
*/
Node *create_num(double num);


Node *create_copy(const Node *node);


Node *create_num(double num) {
    NodeValue value = {0};
    value.dbl = num;
    return create_node(NODE_TYPES::TYPE_NUM, value, nullptr, nullptr);
}


Node *create_copy(const Node *node) {
    if (!node) return nullptr;
    
    return create_node(node -> type, node -> value, create_copy(node -> left), create_copy(node -> right));
}

#define L create_copy(node -> left)
#define R create_copy(node -> right)


#define dL diff(node -> left)
#define dR diff(node -> right)


#define Add(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_ADD}, left, right)
#define Sub(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_SUB}, left, right)
#define Mul(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_MUL}, left, right)
#define Div(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_DIV}, left, right)
#define Pow(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_POW}, left, right)
#define Log(left, right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_LOG}, left, right)
#define Sin(right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_SIN}, nullptr, right)
#define Cos(right) create_node(NODE_TYPES::TYPE_OP, {OPERATORS::OP_COS}, nullptr, right)


Node *diff(const Node *node) {
    switch(node -> type) {
        case NODE_TYPES::TYPE_NUM:
            return create_num(0);
        case NODE_TYPES::TYPE_VAR:
            return create_num(1);
        case NODE_TYPES::TYPE_OP:
            switch (node -> value.op) {
                case OPERATORS::OP_ADD:
                    return Add(dL, dR);
                case OPERATORS::OP_SUB:
                    return Add(dL, dR);
                case OPERATORS::OP_MUL:
                    return Add(Mul(dL, R), Mul(dR, L));
                case OPERATORS::OP_DIV:
                    return Div(Sub(Mul(dL, R), Mul(dR, L)), Mul(R, R));
                case OPERATORS::OP_POW:
                    return Mul(Mul(R, Pow(L, Sub(R, create_num(1)))), dL);
                case OPERATORS::OP_LOG:
                    return Mul(Div(create_num(1), Mul(R, Log(create_num(exp(1)), L))), dR);
                case OPERATORS::OP_SIN:
                    return Mul(Cos(R), dR);
                case OPERATORS::OP_COS:
                    return Mul(Mul(create_num(1), Sin(R)), dR);
                default: 
                    return nullptr;
            }
        default:
            return nullptr;
    }
}
