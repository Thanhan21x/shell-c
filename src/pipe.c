#include "pipe.h"
#include "commands.h"

#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int piping(char **args) {
  char **left;
  char **right;

  for (int i = 0; args[i]; i++) {
    if (!strcmp(args[i], "|")) {
      args[i] = NULL;
      left = args;
      right = args + i + 1;
      break;
    }
  }

  int pfds[2];

  pipe(pfds);

  pid_t p = fork();

  if (p == 0) {
    // child process
    close(1); // close normal stdout
    dup(pfds[1]); // makse stdout same as pfds[1]
    close(pfds[0]); // we don't need this
    execvp(left[0], left);

  } else if (p > 0) {
    close(0); // close normal stdin
    dup(pfds[0]); // makse stdout same as pfds[0]
    close(pfds[1]); // we don't need this
    exec_command(right);
  }

  return 0;

}
