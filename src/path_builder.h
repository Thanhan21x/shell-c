#ifndef PATH_H
#define PATH_H
#include <stdio.h>

typedef struct PATH_BUILDER_STRUCT {
  char *src;
  int cursor;
  size_t size;
  char current;
} path_builder_t;

path_builder_t *init_builder(char *src);

void path_builder_advance(path_builder_t *builder);

char *build_path(char *src, FILE *file);

#endif // !PATH_H
