#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.hpp"
#include "io.hpp"
#include "dif.hpp"


/**
 * \brief Get program parameters from file
 * \param [in] filename             Path to file
 * \param [out] tree                Tree to read into
 * \param [out] point               Point to calculate value
 * \param [out] taylor_order        Taylor series order
 * \param [out] min_value           Graphic min value
 * \param [out] max_value           Graphic max value
*/
int get_parameters(const char *filename, Tree *tree, double *point, int *order, int *taylor_order, double *min_value, double *max_value);




int main(int argc, char *argv[]) {
    Tree tree = {};
    double point = 0, min_value = 0, max_value = 0;
    int order = 0, taylor_order = 0;

    if (argc != 2) {
        printf("Wrong command line arguments!");
        return 1;
    }
    else if (get_parameters(argv[1], &tree, &point, &order, &taylor_order, &min_value, &max_value)) {
        return 1;
    }

    create_derivative_doc(&tree, order, point, taylor_order, min_value, max_value);

    tree_destructor(&tree);

    printf("Tree!");

    return 0;
}


int get_parameters(const char *filename, Tree *tree, double *point, int *order, int *taylor_order, double *min_value, double *max_value) {
    FILE *file = fopen(filename, "r");

    char param[10] = "";

    for (int i = 0; i < 6; i++) {
        fscanf(file, "%s", param);

        if (strcmp(param, "func") == 0) {
            char *buffer = (char *) calloc(512, sizeof(char));

            fgets(buffer, 512, file);
            
            read_tree(tree, buffer);
            
            free(buffer);
        }
        else if (strcmp(param, "point") == 0) {
            fscanf(file, "%lg", point);
        }
        else if (strcmp(param, "order") == 0) {
            fscanf(file, "%i", order);
        }
        else if (strcmp(param, "taylor_order") == 0) {
            fscanf(file, "%i", taylor_order);
        }
        else if (strcmp(param, "min_value") == 0) {
            fscanf(file, "%lg", min_value);
        }
        else if (strcmp(param, "max_value") == 0) {
            fscanf(file, "%lg", max_value);
        }
        else {
            printf("Unknown command \"%s!\"", param);
            return 1; 
        }
    }

    return 0;
}
