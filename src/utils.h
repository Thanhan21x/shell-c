#ifndef UTILS_H

#include <stdbool.h>

#define UTILS_H

bool includes(char **items, char *item);

char *str_join_from(char **items, int from, char *delimeter);

char *substr(char *str, int start, int size);

char *get_redirect_file(char **args);

void cleanup_args(char **args);

#endif
