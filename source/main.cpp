#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "io.hpp"
#include "log.hpp"


int main() {
    Tree tree = {};

    char *buffer = nullptr;

    read_tree(&tree, "debug/string-tree.txt", &buffer);

    FILE *log = fopen("debug/log.html", "w");

    DUMP_IT(log, &tree, fprintf(log, "Dumping tree!"));

    fclose(log);

    // write_tree(&tree, "debug/string-tree.txt");

    tree_destructor(&tree);

    free(buffer);

    printf("Tree!");

    return 0;
}
