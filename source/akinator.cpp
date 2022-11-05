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


int akinator_define(Tree *tree) {
    printf("Enter object name ");

    char name[100] = "";
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0'; // remove '\n' from end of name

    Node *path[10];
    if (!find_in_tree(tree, name, path)) return 1;

    printf("%s is ", name);

    for(int i = 0; path[i] -> left && path[i] -> right; i++) {
        if (path[i + 1] == path[i] -> left)
            printf("%s ", path[i] -> data);
        else
            printf("not %s ", path[i] -> data);
    }

    printf("\n");

    return 0;
}
