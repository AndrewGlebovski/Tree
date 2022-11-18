#include <stdio.h>
#include "tree.hpp"
#include "dif.hpp"


/**
 * \brief Shortcut for creating numeric type node
 * \param [in] num Node will be init with this value
 * \return New node
*/
Node *create_num(double num);


Node *create_num(double num) {
    NodeValue value = {0};
    value.dbl = num;
    return create_node(NODE_TYPES::NUM, value, nullptr, nullptr);
}


#define L create_node(node -> left -> type, node -> left -> value, nullptr, nullptr)
#define R create_node(node -> right -> type, node -> right -> value, nullptr, nullptr)


#define dL diff(node -> left)
#define dR diff(node -> right)


#define Add(left, right) create_node(NODE_TYPES::OP, {OPERATORS::OP_ADD}, left, right)
#define Sub(left, right) create_node(NODE_TYPES::OP, {OPERATORS::OP_SUB}, left, right)
#define Mul(left, right) create_node(NODE_TYPES::OP, {OPERATORS::OP_MUL}, left, right)
#define Div(left, right) create_node(NODE_TYPES::OP, {OPERATORS::OP_DIV}, left, right)


Node *diff(const Node *node) {
    switch(node -> type) {
        case NODE_TYPES::NUM:
            return create_num(0);
        case NODE_TYPES::VAR:
            return create_num(1);
        case NODE_TYPES::OP:
            switch (node -> value.op) {
                case OPERATORS::OP_ADD:
                    return Add(dL, dR);
                case OPERATORS::OP_SUB:
                    return Add(dL, dR);
                case OPERATORS::OP_MUL:
                    return Add(Mul(dL, R), Mul(dR, L));
                case OPERATORS::OP_DIV:
                    return Div(Sub(Mul(dL, R), Mul(dR, L)), Mul(R, R));
                default: 
                    return nullptr;
            }
        default:
            return nullptr;
    }
}
