/**
 * \brief Reads tree from file
 * \param [out] tree     Reads file in this tree
 * \param [in]  filepath Path to input file
 * \return Non zero value means error
*/
int read_tree(Tree *tree, const char *filepath);


/**
 * \brief Prints tree to file in specific format (preorder)
 * \param [out] tree     Prints this tree in file
 * \param [in]  filepath Path to output file
 * \return Non zero value means error
*/
int write_tree(Tree *tree, const char *filepath);
