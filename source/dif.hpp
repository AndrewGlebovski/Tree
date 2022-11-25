/**
 * \brief Differentiates expression tree
 * \param [in] node Pointer to expression tree
 * \return New tree
*/
Node *diff(const Node *node);


/**
 * \brief Calculates constant expressions
 * \param [in] node Check will start from this branch
*/
void optimize(Node *node);


/**
 * \brief Calculates expression tree value based on x value
 * \param [in] node Calculation will start from this branch
 * \param [in] x    Value of function argument
 * \return Function value
*/
double calc_value(const Node *node, double x);


void create_derivative_doc(Tree *tree, int order, double point);
