#include <stdio.h>
#include "tree.hpp"
#include "io.hpp"
#include "log.hpp"
#include "akinator.hpp"


int main() {
    Tree tree = {};

    read_tree(&tree, "debug/string-tree.txt");

    FILE *log = fopen("debug/log.html", "w");

    DUMP_IT(log, &tree, fprintf(log, "Dumping tree!"));

    fclose(log);

    // write_tree(&tree, "debug/string-tree.txt");

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}
