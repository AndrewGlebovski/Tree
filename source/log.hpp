/**
 * \file
 * \brief Log header
*/


/**
 * \brief Creates enumerated list of dot files and images, also adds information to hmtl file
 * \param [in]  tree Tree to print
 * \param [out] log  Log file output
 * \return Non zero value means error
*/
int graphic_dump(Tree *tree, FILE *log);


/**
 * \brief Generates text file for dot
 * \param [in]  tree Tree to print
 * \param [out] log  Log file output
 * \return Non zero value means error
*/
int generate_file(Tree *tree, FILE *file);


/**
 * \brief Generates image using dot call
 * \param [in]  input  Path to text file for dot
 * \param [out] output Path to output image
 * \return Non zero value means error
*/
int generate_image(const char *input, const char *output);


/**
 * \brief Pause program so you can watch selected image
 * \param [in]  filepath Path to image file
 * \return Non zero value means error
*/
int show_image(const char *filepath);


#ifdef HTML_LOG

/**
 * \brief Generates numerated dumps and log file in html format
 * \param [out] log   Log file output
 * \param [in]  tree  Tree to print
 * \param [in]  title Current dump title
*/
#define DUMP_IT(log, tree, title, ...)                                                 \
do {                                                                                   \
    if (log) {                                                                         \
        fprintf(log, "<div style=\"width:95%%\">\n");                                  \
        fprintf(log, "<h1>");                                                          \
        title;                                                                         \
        fprintf(log, "</h1>\n");                                                       \
        fprintf(log, "<xmp style=\"font-size: auto\">\n");                             \
        fprintf(log, "%s(%i) in %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);       \
        fprintf(log, "</xmp>\n");                                                      \
        graphic_dump(tree, log);                                                       \
        __VA_ARGS__;                                                                   \
        fprintf(log, "</div>\n");                                                      \
        fprintf(log, "<hr>\n");                                                        \
    }                                                                                  \
} while(0)

#else

/**
 * \brief Generates numerated dumps
 * \param [in]  tree  Tree to print
*/
#define DUMP_IT(tree, ...)                                                             \
graphic_dump(tree, nullptr)

#endif
