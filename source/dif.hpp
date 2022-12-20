/**
 * \brief Differentiates expression tree
 * \param [in]  node Pointer to expression tree
 * \param [out] file Output stream
 * \return New tree
*/
Node *diff(const Node *node, FILE *file, const char *phrases[], int phrases_size);


/**
 * \brief Calculates constant expressions
 * \param [in]  node Check will start from this branch
 * \param [out] file Output stream
*/
void optimize(Node *node, FILE *file, const char *phrases[], int phrases_size);


/**
 * \brief Calculates expression tree value based on x value
 * \param [in] node Calculation will start from this branch
 * \param [in] x    Value of function argument
 * \return Function value
*/
double calc_value(const Node *node, double x);


/**
 * \brief Creates differentiate tutorial in .tex format
 * \param [in] tree         Expression tree
 * \param [in] order        Differentiates to this order
 * \param [in] point        Point where calculates value
 * \param [in] taylor_order Order of the taylor series
 * \param [in] min_value    Graphic function min value
 * \param [in] max_value    Graphic function max value
*/
void create_derivative_doc(Tree *tree, int order, double point, int taylor_order, double min_value, double max_value);


#define FUNC_GRAPH

#ifdef FUNC_GRAPH

/**
 * \brief Creates function graph on [-10;10]
 * \param [in] tree     Function to draw
 * \param [in] filename Image name
*/
void create_func_graph(Tree *tree, const char *filname, double min_value, double max_value);

#endif
