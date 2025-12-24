#include "commands.h"

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");

    char input[1024];
    fgets(input, sizeof(input), stdin);
 
    // Remove the trailing newline
    input[strcspn(input, "\n")] = '\0';
 
    int _argc = 0;
    char *_argv[1024];
    get_arg(input, &_argc, _argv);

    // Take the command which is the word before the first space
    char command[64];
    strcpy(command, _argv[0]);

    char exec_path[64];

    if (strlen(command) < 1) {
      continue;

    } else if (!strcmp(command, "exit")) {
      return 0;

    } else if (!strcmp(command, "echo")) {
      for (int i = 1; i < _argc; i++) {
        printf("%s ", _argv[i]);
      }
      printf("\n");

    } else if (!strcmp(command, "type")) {
      command_type(_argc, _argv);
      
    } else if (!strcmp(command, "pwd")) {
      char cwd[256];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);

    } else if (!strcmp(command, "cd")) {
      int status;
      if (_argc == 1 || !strcmp(_argv[1], "~")) {
        status  = chdir(getenv("HOME"));
      } else  {
        status = chdir(_argv[1]);
      }
      if (status == -1) {
        fprintf(stderr, "cd: %s: No such file or directory\n", _argv[1]);
      }

    } else if (is_executable_command(command, exec_path)) {
      command_run(exec_path, command, _argv);

    } else {
      printf("%s: command not found\n", command);
    }
  }

  return 0;
}
