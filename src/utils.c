#include "utils.h"
#include <stdlib.h>
#include <string.h>

bool includes(char **items, char *item) {
  for (int i = 0; items[i] != NULL; i++) {
    if (strcmp(items[i], item) == 0) {
      return true;
    }
  }

  return false;
}

char *get_redirect_file(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
    if (!strcmp(args[i], ">") || !strcmp(args[i], "1>")) {
      char *filename = strdup(args[i+1]);
      args[i] = NULL;
      return filename;
    }
  }
  return NULL;

}

char *str_join_from(char **items, int from, char *delimeter) {
  char *buffer = calloc(1, sizeof(char *));
  buffer[0] = '\0';

  int delimeter_length = strlen(delimeter);

  for (int i = from; items[i] != NULL; i++) {
    int length = strlen(buffer) + strlen(items[i]);
    bool include_delimeter = strlen(buffer) > 0;

    buffer = realloc(buffer, (length + include_delimeter * delimeter_length) *
                                 sizeof(char *));

    if (include_delimeter) {
      strcat(buffer, delimeter);
    }

    strcat(buffer, items[i]);
  }

  return buffer;
}

char *substr(char *str, int start, int size) {
  char *output = malloc(size * sizeof(char *));

  *str += start;

  while (size--) {
    *output++ = *str++;
  }

  return output;
}

void cleanup_args(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
    free(args[i]);
  }

  free(args);
}
