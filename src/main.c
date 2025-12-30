#include "args.h"
#include "commands.h"
#include "utils.h"
#include "completer.h"
#include "history.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

#include <readline/history.h>
#include <readline/readline.h>

int main(int argc, char *argv[]) {
    
  // Enable history
  using_history();
  // Bind the completer;
  initialize_readline();

  char *histfile = strdup(getenv("HISTFILE"));
  if (histfile) {
    add_history_from_file(histfile);
  }

  while (true) {

    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);
    setbuf(stdout, NULL);

    char prompt[MAX_INPUT];
    snprintf(prompt, sizeof(prompt), "$ ");

    char *input = readline(prompt);

    // Check for EOF.
    if (!input) {
      break;
    }

    add_history(input);

    if (strlen(input) < 1) {
      continue;
    }

    char **args = parse_input(input);

    free(input);

    if (args == NULL) {
      continue;
    }

    exec_command(args);

    cleanup_args(args);

    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);

    close(saved_stdout);
  }

  if (histfile) {
    write_history_to_file(histfile);
  }
  free(histfile);

  return 0;
}
