#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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


/**
 * \brief Accept 'y' as yes and 'n' as no
 * \note Non zero value means positiv answer
*/
int get_answer();




int get_answer() {
    int answer = getchar();

    while (answer != 'n' && answer != 'y') 
        answer = getchar();

    while (!isspace(getchar())); // skip 'n' symbol

    return answer - 'n';
}


int ask(Node *node) {
    if (!node) return 1;

    if (!node -> left && !node -> right) {
        printf("%s, isn't it?\n", node -> data);

        if (get_answer()) 
            printf("I'm superior once more!\n");
        else 
            add_object(node);

        return 0;
    }

    printf("%s? ", node -> data);

    if (get_answer()) 
        return ask(node -> left);
    else
        return ask(node -> right);
}


void add_object(Node *node) {
    printf("I don't know other person. Can you write its name? ");

    char *name = (char *) calloc(100, sizeof(char)), *question = (char *) calloc(100, sizeof(char));

    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0'; // remove '\n' from end of name

    printf("What's the difference between %s and %s? %s is ", name, node -> data, name);

    fgets(question, 100, stdin);
    question[strlen(question) - 1] = '\0'; // remove '\n' from end of question

    printf("The more you know!\n");

    node -> left = create_node(name);
    node -> right = create_node(node -> data);
    node -> data = question;
}


int akinator_guess(Tree *tree) {
    return ask(tree -> root);
}


#define PRINT_PROPERTY(path, iter)                  \
do {                                                \
    if (path[iter + 1] == path[iter] -> left)       \
        printf("%s ", path[iter] -> data);          \
    else                                            \
        printf("not %s ", path[iter] -> data);      \
} while(0)


// fgets() also writes '\n' to buffer
#define READ_STRING(var, size)                      \
    char var[size] = "";                            \
    fgets(var, size, stdin);                        \
    var[strlen(var) - 1] = '\0';


int akinator_define(Tree *tree) {
    printf("Enter object name ");

    READ_STRING(name, 100)

    Node *path[10];
    if (!find_in_tree(tree, name, path)) return 1;

    printf("%s is ", name);

    for(int i = 0; path[i] -> left && path[i] -> right; i++) PRINT_PROPERTY(path, i);

    printf("\n");

    return 0;
}


int akinator_compare(Tree *tree) {
    READ_STRING(name1, 100)
    READ_STRING(name2, 100)

    Node *path1[10], *path2[10];
    if (!find_in_tree(tree, name1, path1)) return 1;
    if (!find_in_tree(tree, name2, path2)) return 1;

    printf("%s and %s are both ", name1, name2);

    int id = 0;

    for(; path1[id + 1] == path2[id + 1]; id++) PRINT_PROPERTY(path1, id);

    printf("but %s is ", name1);

    for(int i = id; path1[i] -> left && path1[i] -> right; i++) PRINT_PROPERTY(path1, i);

    printf("and %s is ", name2);

    for(int i = id; path2[i] -> left && path2[i] -> right; i++) PRINT_PROPERTY(path2, i);

    printf("\n");

    return 0;
}


#undef PRINT_PROPERTY


#undef READ_STRING
