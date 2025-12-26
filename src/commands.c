#include "commands.h"

void command_cd(char **args) {
  int status;
  if (!strcmp(args[1], "~")) {
    status  = chdir(getenv("HOME"));
  } else  {
    status = chdir(args[1]);
  }
  if (status == -1) {
    fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
  }
}

void command_pwd() {
  char cwd[256];
  getcwd(cwd, sizeof(cwd));
  printf("%s\n", cwd);
}

void command_echo(char **args) {
  int i = 1;
  while (args[i] != NULL) {
    printf("%s ", args[i++]);
  }
  printf("\n");
}

void command_type(char **args) {
  int i = 1;
  if (is_builtin_command(args[i])) {
    printf("%s is a shell builtin\n", args[i]);
  } else {
    char exec_path[128];
    if (is_executable_command(args[i], exec_path)) {
    // copy the executable's path to path
    printf("%s is %s/%s\n", args[i], exec_path, args[i]);

    } else {
    printf("%s: not found\n", args[i]);
    }
  }
}

void command_run(char **args, char *path) {
  char full_path[128];
  snprintf(full_path, sizeof(full_path), "%s/%s", path, args[1]);

  pid_t pid = fork();

  // in child process
  if (pid == 0) {
    execv(full_path, args);
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


