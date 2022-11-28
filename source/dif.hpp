/**
 * \brief Differentiates expression tree
 * \param [in]  node Pointer to expression tree
 * \param [out] file Output stream
 * \return New tree
*/
Node *diff(const Node *node, FILE *file);


/**
 * \brief Calculates constant expressions
 * \param [in]  node Check will start from this branch
 * \param [out] file Output stream
*/
void optimize(Node *node, FILE *file);


/**
 * \brief Calculates expression tree value based on x value
 * \param [in] node Calculation will start from this branch
 * \param [in] x    Value of function argument
 * \return Function value
*/
double calc_value(const Node *node, double x);


/**
 * \brief Creates differentiate tutorial in .tex format
 * \brief [in] tree  Expression tree
 * \brief [in] order Differentiates to this order
 * \brief [in] point Point where calculates value
*/
void create_derivative_doc(Tree *tree, int order, double point);


// #define FUNC_GRAPH Включай только если у тебя есть matplotlib на компе

#ifdef FUNC_GRAPH

/**
 * \brief Creates function graph on [-10;10]
 * \param [in] tree     Function to draw
 * \param [in] filename Image name
*/
void create_func_graph(Tree *tree, const char *filname);

#endif
