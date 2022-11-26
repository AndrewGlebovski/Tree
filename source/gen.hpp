DEF_GEN(ADD, Add(dL, dR),

    CONST_CHECK()
    else TEMPLATE(right, left, 0)
    else TEMPLATE(left, right, 0),

    calc_value(node -> left, x) + calc_value(node -> right, x)
)
DEF_GEN(SUB, Sub(dL, dR),

    CONST_CHECK()
    else TEMPLATE(left, right, 0),

    calc_value(node -> left, x) - calc_value(node -> right, x)
)
DEF_GEN(MUL, Add(Mul(dL, R), Mul(dR, L)),

    CONST_CHECK()
    else if (IS_NUM(right, 0) || IS_NUM(left, 0)) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = 0;

        FREE(node -> left);
        FREE(node -> right);
    }
    else TEMPLATE(right, left, 1)
    else TEMPLATE(left, right, 1),

    calc_value(node -> left, x) * calc_value(node -> right, x)
)
DEF_GEN(DIV, Div(Sub(Mul(dL, R), Mul(dR, L)), Mul(R, R)), 

    CONST_CHECK()
    else TEMPLATE(right, left, 1)
    else if (IS_TYPE(node -> left, VAR) && IS_TYPE(node -> right, VAR)) {
        if (node -> left -> value.var == node -> right -> value.var) {
            node -> type = NODE_TYPES::TYPE_NUM;
            node -> value.dbl = 1;

            FREE(node -> left);
            FREE(node -> right);
        }
    },

    calc_value(node -> left, x) / calc_value(node -> right, x)
)
DEF_GEN(POW, // Лютейший говнокод я сам знаю
    ((IS_TYPE(node -> left, VAR) || IS_TYPE(node -> left, OP)) && (IS_TYPE(node -> right, VAR) || IS_TYPE(node -> right, OP))) ?
        Mul(Pow(L, R), Add(Mul(dL, Ln(R)), Div(Mul(dR, L), R)))
    : (IS_TYPE(node -> left, VAR) || IS_TYPE(node -> left, OP)) ?
        Mul(Mul(R, Pow(L, Sub(R, create_num(1)))), dL) 
    :
        Mul(Mul(Pow(L, R), Ln(L)), dR),
    
    CONST_CHECK()
    else if (IS_NUM(right, 1)) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = 1;

        FREE(node -> left);
        FREE(node -> right);
    }
    else TEMPLATE(right, left, 1),

    pow(calc_value(node -> left, x), calc_value(node -> right, x))
)
DEF_GEN(LOG, Div(dR, Mul(R, Ln(L))),

    CONST_CHECK(),

    log(calc_value(node -> right, x)) / log(calc_value(node -> left, x))
)
DEF_GEN(SIN, Mul(Cos(R), dR),

    if (IS_TYPE(node -> right, NUM)) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = sin(node -> right -> value.dbl);

        FREE(node -> right);
    },

    sin(calc_value(node -> right, x))
)
DEF_GEN(COS, Mul(Mul(create_num(-1), Sin(R)), dR),

    if (IS_TYPE(node -> right, NUM)) {
        node -> type = NODE_TYPES::TYPE_NUM;
        node -> value.dbl = cos(node -> right -> value.dbl);

        FREE(node -> right);
    },

    cos(calc_value(node -> right, x))
)
