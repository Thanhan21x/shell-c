#include "args.h"
#include "commands.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include <readline/history.h>
#include <readline/readline.h>

int main(int argc, char *argv[]) {
  while (true) {
    setbuf(stdout, NULL);

    char prompt[MAX_INPUT];
    snprintf(prompt, sizeof(prompt), "$ ");

    char *input = readline(prompt);

    if (strlen(input) < 1) {
      continue;
    }

    char **args = parse_input(input);

    free(input);

    if (args == NULL) {
      continue;
    }

    if (includes(args, ">") || includes(args, "1>")) {
      char filename[256];
      for (int i = 0; args[i] != NULL; i++) {
        if (!strcmp(args[i], ">") || !strcmp(args[i], "1>")) {
          strcpy(filename, args[i + 1]);
          args[i] = NULL;
          break;
        }
      }

      FILE *fp = fopen(filename, "w+");
      exec_command(args, fp);
      fclose(fp);
    } else {
      exec_command(args, stdout);
    }

    cleanup_args(args);
  }

  return 0;
}
