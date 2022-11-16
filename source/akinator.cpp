#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.hpp"
#include "akinator.hpp"
#include "log.hpp"

const int STRING_SIZE       = 100;
const int PATH_LENGTH       =  10;
const int PRINT_BUFFER_SIZE = 512;


/**
 * \brief Add new object to database
 * \param [in] node New object will be child of this node
 * \note This node data will become question
*/
void add_object(Node *node, char *print_buffer);


/**
 * \brief Accept 'y' as yes and 'n' as no
 * \note Non zero value means positiv answer
*/
int get_answer();


#define PATH_TO_BALCON "C:\\Users\\Tim\\Downloads\\balcon\\balcon.exe"
#define VOICE_NAME "\"Microsoft David Desktop\""


#define PRINT_AND_SOUND(...)                                                                \
do {                                                                                        \
    printf(__VA_ARGS__);                                                                    \
    sprintf(print_buffer, PATH_TO_BALCON " -n " VOICE_NAME " -t " "\"" __VA_ARGS__);   \
    strcat(print_buffer, "\""); \
    system(print_buffer); \
} while(0)


int get_answer() {
    int answer = getchar();

    while (answer != 'n' && answer != 'y') 
        answer = getchar();

    while (!isspace(getchar())); // skip 'n' symbol

    return answer - 'n';
}


void add_object(Node *node, char *print_buffer) {
    PRINT_AND_SOUND("I don't know other person. Can you write its name? ");

    char *name = (char *) calloc(STRING_SIZE, sizeof(char)), *question = (char *) calloc(STRING_SIZE, sizeof(char));

    fgets(name, STRING_SIZE, stdin);
    name[strlen(name) - 1] = '\0'; // remove '\n' from end of name

    PRINT_AND_SOUND("What's the difference between %s and %s? %s is ", name, node -> data, name);

    fgets(question, STRING_SIZE, stdin);
    question[strlen(question) - 1] = '\0'; // remove '\n' from end of question

    PRINT_AND_SOUND("The more you know!\n");

    node -> left = create_node(name);
    node -> right = create_node(node -> data);
    node -> data = question;
}


int akinator_guess(Tree *tree, char *print_buffer) {
    Node *ptr = tree -> root;

    for(; ptr -> left && ptr -> right;) {
        PRINT_AND_SOUND("%s? ", ptr -> data);

        if (get_answer()) ptr = ptr -> left;
        else              ptr = ptr -> right;
    }

    PRINT_AND_SOUND("%s, isn't it?", ptr -> data);

    if (get_answer())
        PRINT_AND_SOUND("I'm superior once more!\n");
    else
        add_object(ptr, print_buffer);

    return 0;
}


#define PRINT_PROPERTY(path, iter)                  \
do {                                                \
    if (path[iter + 1] != path[iter] -> left)       \
        strcat(print_buffer, "not ");               \
    strcat(print_buffer, path[iter] -> data);       \
    strcat(print_buffer, " ");                      \
} while(0)


// fgets() also writes '\n' to buffer
#define READ_STRING(var, size)                      \
    char var[size] = "";                            \
    fgets(var, size, stdin);                        \
    var[strlen(var) - 1] = '\0';


int akinator_define(Tree *tree, char *print_buffer) {
    PRINT_AND_SOUND("Enter object name ");

    READ_STRING(name, STRING_SIZE)

    Node *path[PATH_LENGTH] = {nullptr};
    if (!find_in_tree(tree, name, path)) return 1;

    sprintf(print_buffer, PATH_TO_BALCON " -n " VOICE_NAME " -t " "\"" "%s is ", name); 

    for(int i = 0; path[i] -> left && path[i] -> right; i++) PRINT_PROPERTY(path, i);

    printf("%s\n", print_buffer + strlen(PATH_TO_BALCON " -n " VOICE_NAME " -t " "\""));

    strcat(print_buffer, "\"");

    system(print_buffer);

    return 0;
}


int akinator_compare(Tree *tree, char *print_buffer) {
    PRINT_AND_SOUND("Enter two objects\n");

    READ_STRING(name1, STRING_SIZE)
    READ_STRING(name2, STRING_SIZE)

    Node *path1[PATH_LENGTH] = {nullptr}, *path2[PATH_LENGTH] = {nullptr};
    if (!find_in_tree(tree, name1, path1)) return 1;
    if (!find_in_tree(tree, name2, path2)) return 1;

    sprintf(print_buffer, PATH_TO_BALCON " -n " VOICE_NAME " -t " "\"" "%s and %s are both ", name1, name2);

    int id = 0;

    for(; path1[id + 1] == path2[id + 1]; id++) PRINT_PROPERTY(path1, id);

    sprintf(print_buffer + strlen(print_buffer), "but %s is ", name1);

    for(int i = id; path1[i] -> left && path1[i] -> right; i++) PRINT_PROPERTY(path1, i);

    sprintf(print_buffer + strlen(print_buffer), "and %s is ", name2);

    for(int i = id; path2[i] -> left && path2[i] -> right; i++) PRINT_PROPERTY(path2, i);

    printf("%s\n", print_buffer + strlen(PATH_TO_BALCON " -n " VOICE_NAME " -t " "\""));

    strcat(print_buffer, "\"");

    system(print_buffer);

    return 0;
}


#undef PRINT_PROPERTY


#undef READ_STRING


void akinator_start(Tree *tree) {
    char print_buffer[PRINT_BUFFER_SIZE] = "";
    char cmd[STRING_SIZE]                = "";

    // system("\"C:\\Users\\Tim\\Downloads\\balcon\\balkon.exe\" -n \"Microsoft David Desktop\" -t \"Hello, world!\"");

    printf("Available commands\n");
    printf("guess   - My powerfull AI will ask you several yes/no question\n");
    printf("define  - My powerfull AI will show you object properties. Make sure object exists\n");
    printf("compare - My powerfull AI will show commons and differences of objects. Make sure objects exist\n");

    while (strcmp(cmd, "exit")) {
        PRINT_AND_SOUND("Enter command ");
        scanf("%s", cmd);
        getchar(); // skip '\n' symbol

        if (strcmp(cmd, "guess") == 0) {
            if (akinator_guess(tree, print_buffer))
                PRINT_AND_SOUND("Something went wrong!\n");
        }

        else if (strcmp(cmd, "define") == 0) {
            if (akinator_define(tree, print_buffer))
                PRINT_AND_SOUND("Object not found\n");
        }

        else if (strcmp(cmd, "compare") == 0) {
            if (akinator_compare(tree, print_buffer))
                PRINT_AND_SOUND("Object not found!\n");
        }

        else if (strcmp(cmd, "show") == 0) {
            FILE *file = fopen("debug/show.txt", "w");
            generate_file(tree, file);
            fclose(file);
            generate_image("debug/show.txt", "debug/img.png");
            show_image("debug/img.png");
        }
    }
}
