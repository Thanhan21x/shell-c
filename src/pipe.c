#include "pipe.h"
#include <stddef.h>
#include <string.h>

int piping(char **args) {
  int pfds[2];


  char **left = args;
  char **right;
  int idx = 0;

  for (int i = 0; args[i]; i++) {
    if (!strcmp(args[i], "|")) {
      args[i] = NULL;
      left = args;
      right = args + i + 1;
      break;
    }
  }
  return 1;
}
