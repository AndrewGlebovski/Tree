/// Possible function exit codes
typedef enum {
    INVALID_ARG  =  1,       ///< Function got invalid argument
    ALLOC_FAIL   =  2,       ///< Failed to allocate memory
} ERROR_CODES;


/// Node class
struct Node {
    int data = 0;                       ///< Node value
    Node *left = nullptr;               ///< Left child node
    Node *right = nullptr;              ///< Right child node
};


/// Tree class
typedef struct {
    Node *root = nullptr;               ///< Tree root node
    int size = 0;                       ///< Nodes count
} Tree;


/**
 * \brief Constructs tree
 * \param [out] tree Previously initialized tree structure
 * \param [in]  root If not null, then it will be set as root of the tree,
 * otherwise root will be allocated
 * \return Non zero value means error
*/
int tree_constructor(Tree *tree, Node *root = nullptr);


/**
 * \brief Allocates new node
 * \param [in] value New node's value
 * \param [in] left  New node's left child
 * \param [in] right New node's right child
 * \return Pointer to node or nullptr if allocates fail
*/
Node *create_node(int value, Node *left = nullptr, Node *right = nullptr);


/**
 * \brief Destructs tree
 * \param [in] tree All tree nodes will be free
 * \return Non zero value means error
*/
int tree_destructor(Tree *tree);


/**
 * \brief Prints tree
 * \param [in] node Tree to print
 * \param [out] stream Output stream
*/
void print_tree(Tree *tree, FILE *stream = stdout);
