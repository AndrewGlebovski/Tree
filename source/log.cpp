/**
 * \file
 * \brief Log source
*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.hpp"
#include "log.hpp"


/// Writes node and it's child connections
void write_record(FILE *file, Node *node);




int generate_file(Tree *tree, FILE *file) {
    fprintf(file, "digraph G {\n");

    fprintf(file, "    rankdir=HB\n");

    fprintf(file, "    node[shape=record, color=\"#355250\", fontsize=14, fontcolor=\"#355250\", style=\"filled\", fillcolor=\"#daf9f4\"];\n");
    fprintf(file, "    edge[color=\"#019aa2\"];\n");

    write_record(file, tree -> root);

    fprintf(file, "}\n");

    return 0;
}


void write_record(FILE *file, Node *node) {
    fprintf(file, "    \"e%p\"[label=\"{<index> %-p | %s}\"];\n", node, node, node -> data);

    if (node -> left) {
        fprintf(file, "    \"e%p\" -> \"e%p\";\n", node, node -> left);
        write_record(file, node -> left);
    }

    if (node -> right) {
        fprintf(file, "    \"e%p\" -> \"e%p\";\n", node, node -> right);
        write_record(file, node -> right);
    }
}


int generate_image(const char *input, const char *output) {
    char cmd[20 + 2 * _MAX_PATH] = "";

    sprintf(cmd, "dot %s -o %s -Tpng", input, output);

    return system(cmd);
}


int show_image(const char *filepath) {
    char cmd[10 + _MAX_PATH] = "";

    sprintf(cmd, "start %s", filepath);

    system(cmd);

    return system("pause");
}


#define DUMP_DIRECTORY "debug"
#define DOT_FILENAME "dot-"
#define IMG_FILENAME "img-"


int graphic_dump(Tree *tree, FILE *log) {
    static int dump_index = 0;

    char dot[FILENAME_MAX] = "", img[FILENAME_MAX] = "";

    sprintf(dot, DUMP_DIRECTORY "/" DOT_FILENAME "%i.txt", dump_index);
    sprintf(img, DUMP_DIRECTORY "/" IMG_FILENAME "%i.png", dump_index);

    FILE *file = fopen(dot, "w");

    generate_file(tree, file);

    fclose(file);

    generate_image(dot, img);

    fprintf(log, "<img src=\"" IMG_FILENAME "%i.png\" alt=\"WTF\" style=\"width: auto\">\n", dump_index);

    dump_index++;

    return 0;
}

