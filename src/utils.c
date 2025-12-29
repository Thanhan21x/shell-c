#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

bool includes(char **items, char *item) {
  for (int i = 0; items[i] != NULL; i++) {
    if (strcmp(items[i], item) == 0) {
      return true;
    }
  }

  return false;
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
  // append newline char
  buffer[strlen(buffer)] = '\n';

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

void ensure_parent_dirs(const char *path) {
  char tmp[512];
  strncpy(tmp, path, sizeof(path));
  tmp[sizeof(tmp) -1] = '\0';

  for (char *p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';
      mkdir(tmp, 0755);
      *p = '/';
    }
  }
}

extern const char *builtins[]; // from command.c
char **list_executable(char first_char) {
  char **execs = malloc(sizeof(char) * 1024 * 256);
  int idx = 0;

  char *paths = strdup(getenv("PATH"));
  if (!paths)
    return NULL;

  char *saveptr;
  char *path = strtok_r(paths, ":", &saveptr);

  while (path) {
    DIR *dir = opendir(path);
    if (!dir) {
        path = strtok_r(NULL, ":", &saveptr);
        continue;   // DO NOT exit
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] == '.' || entry->d_name[0] != first_char)
        continue;

      char fullpath[PATH_MAX];
      snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

      if (access(fullpath, X_OK) == 0) {
        execs[idx++] = strdup(entry->d_name);
      }
    }

    closedir(dir);
    path = strtok_r(NULL, ":", &saveptr);
  }

  for (int i = 0; builtins[i]; i++) {
    execs[idx++] = strdup(builtins[i]);
  }

  execs[idx] = NULL;

  return execs;

}
