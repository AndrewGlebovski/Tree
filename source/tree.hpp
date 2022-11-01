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
