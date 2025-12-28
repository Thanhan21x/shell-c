#ifndef UTILS_H

#include <stdbool.h>

#define UTILS_H

bool includes(char **items, char *item);

char *str_join_from(char **items, int from, char *delimeter);

char *substr(char *str, int start, int size);

void cleanup_args(char **args);

void ensure_parent_dirs(const char *path);

char **list_executable(char first_char);

#endif
