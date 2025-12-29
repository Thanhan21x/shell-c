#include "path_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

path_builder_t *init_builder(char *src) {
  path_builder_t *builder = malloc(sizeof(struct PATH_BUILDER_STRUCT));
  builder->src = src;
  builder->size = strlen(src);
  builder->cursor = 0;
  builder->current = src[0];

  return builder;
}

void path_builder_advance(path_builder_t *builder) {
  if (builder->cursor < builder->size && builder->current != '\0') {
    builder->cursor++;
    builder->current = builder->src[builder->cursor];
  }
}

char *build_path(char *src) {
  if (src == NULL || src[0] == '~') {
    char *home = strdup(getenv("HOME"));

    if (home == NULL) {
      fprintf(stderr, "cd: HOME is not set\n");
      return NULL;
    }

    if (src == NULL) {
      return home;
    }
    char *input = strdup(src);
    char *profile = strtok(input, "/");
    char *path = strtok(NULL, "");

    char *buffer;

    if (path != NULL) {
      int length = strlen(home) + strlen(path) + 2;
      buffer = malloc(length * sizeof(char *));
      snprintf(buffer, length, "%s/%s", home, path);
    } else {
      int length = strlen(home) + 1;
      buffer = malloc(length * sizeof(char *));
      snprintf(buffer, length, "%s", home);
    }

    free(home);
    free(input);

    return buffer;

  }
  return strdup(src);
}

