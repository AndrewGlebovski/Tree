#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "akinator.hpp"


/**
 * \brief Asks current node question
 * \param [in] node Node to get question from
 * \return Non zero value means error
 * \note Left child is yes, right child is no
*/
int ask(Node *node);


/**
 * \brief Add new object to database
 * \param [in] node New object will be child of this node
 * \note This node data will become question
*/
void add_object(Node *node);
