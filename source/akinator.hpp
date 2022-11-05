/**
 * \brief Can guess object from tree using yes/no questions
 * \param [in] tree Object database
 * \return Non zero value means error
*/
int akinator_guess(Tree *tree);


/**
 * \brief Finds object in tree and prints its properties
 * \param [in] tree Object database
 * \return Non zero value means error
*/
int akinator_define(Tree *tree);


/**
 * \brief Compares two objects in tree and prints their commons and differences
 * \param [in] tree Object database
 * \return Non zero value means error
*/
int akinator_compare(Tree *tree);
