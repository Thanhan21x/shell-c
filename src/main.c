#include "commands.h"
#include "args.h"

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");

    char input[1024];
    fgets(input, sizeof(input), stdin);
 
    // Remove the trailing newline
    input[strcspn(input, "\n")] = '\0';

    char **args = parse_input(input);


    char exec_path[64];

    if (strlen(args[0]) < 1) {
      continue;

    } else if (!strcmp(args[0], "exit")) {
      return 0;

    } else if (!strcmp(args[0], "echo")) {
      command_echo(args);

    } else if (!strcmp(args[0], "type")) {
      command_type(args);
      
    } else if (!strcmp(args[0], "pwd")) {
      command_pwd();

    } else if (!strcmp(args[0], "cd")) {
      command_cd(args);

    } else if (is_executable_command(args[0], exec_path)) {
      command_run(args, exec_path);

    } else {
      printf("%s: command not found\n", args[0]);
    }
  }

  return 0;
}
