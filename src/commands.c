#include "commands.h"

void command_type(int _argc, char *_argv[]) {
  for (int i = 1; i < _argc; i++) {
    if (is_builtin_command(_argv[i])) {
      printf("%s is a shell builtin\n", _argv[i]);
    } else {
      char exec_path[128];
      if (is_executable_command(_argv[i], exec_path)) {
      // copy the executable's path to path
      printf("%s is %s/%s\n", _argv[i], exec_path, _argv[i]);

      } else {
      printf("%s: not found\n", _argv[i]);
      }
    }
  }
}

void command_run(char *path, char *cmd, char *_argv[]) {
  char full_path[128];
  snprintf(full_path, sizeof(full_path), "%s/%s", path, cmd);

  pid_t pid = fork();

  // in child process
  if (pid == 0) {
    execv(full_path, _argv);
    perror("execv");
    _exit(1);
  // in parent process (this program)
  } else if (pid > 0) {
    // wait for the chil to exit
    waitpid(pid, NULL, 0); // wait for child process done
  } else {
    perror("fork");
  }
}


