/**
 * \brief Reads tree
 * \param [out] tree   Save input in this tree
 * \param [in]  buffer Char buffer to read from
 * \return Non zero value means error
*/
int read_tree(Tree *tree, char *buffer);


/**
 * \brief Prints tree to file in specific format (preorder)
 * \param [out] tree     Prints this tree in file
 * \param [in]  filepath Path to output file
 * \return Non zero value means error
*/
int write_tree(Tree *tree, const char *filepath);
