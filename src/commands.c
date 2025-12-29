#include "commands.h"
#include "path_builder.h"
#include "utils.h"
#include "redirect.h"
#include "pipe.h"

#include "sys/wait.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

const char *builtins[] = {"cd", "echo", "exit", "pwd", "type", NULL};

bool is_builtin_command(char *command) {
  return includes((char **)builtins, command);
}

int exec_builtin_cd(char **args) {
  char *path = build_path(args[1]);

  if (path == NULL) {
    return 1;
  }

  int code = chdir(path);
  if (code != 0) {
    fprintf(stderr, "cd: %s: No such file or directory\n", path);
  }

  free(path);

  return code;
}

int exec_builtin_echo(char **args) {
  char *output = str_join_from(args, 1, " ");

  fprintf(stdout, "%s\n", output);
  fflush(stdout);

  free(output);

  return 0;
}

int exec_builtin_exit(char **args) {
  int exit_code = args[1] != NULL ? atoi(args[1]) : 0;

  cleanup_args(args);

  exit(exit_code);
}

int exec_builtin_pwd() {
  char buffer[512];
  if (getcwd(buffer, sizeof(buffer)) != NULL) {
    fprintf(stdout, "%s\n", buffer);
    fflush(stdout);
    return 0;
  }

  fprintf(stderr, "Could not get current working directory");

  return 1;
}

int exec_builtin_type(char **args) {
  if (args[1] == NULL) {
    return 1;
  }

  if (is_builtin_command(args[1])) {
    fprintf(stdout,  "%s is a shell builtin\n", args[1]);
    fflush(stdout);
    return 0;
  }

  char *paths = strdup(getenv("PATH"));

  if (paths == NULL) {
    fprintf(stderr, "%s: not found\n", args[1]);
    return 1;
  }

  char *path = strtok(paths, ":");

  while (path != NULL) {
    char fullpath[PATH_MAX];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, args[1]);

    if (access(fullpath, X_OK) == 0) {

      fprintf(stderr, "%s is %s\n", args[1]);

      free(paths);

      return 0;
    }

    path = strtok(NULL, ":");
  }

  free(paths);

  fprintf(stderr, "%s: not found\n", args[1]);

  return 1;
}

int exec_builtin_command(char **args) {
  if (strcmp(args[0], "cd") == 0) {
    return exec_builtin_cd(args);
  }

  if (strcmp(args[0], "echo") == 0) {
    return exec_builtin_echo(args);
  }

  if (strcmp(args[0], "exit") == 0) {
    return exec_builtin_exit(args);
  }

  if (strcmp(args[0], "pwd") == 0) {
    return exec_builtin_pwd();
  }

  if (strcmp(args[0], "type") == 0) {
    return exec_builtin_type(args);
  }

  return 1;
}

int exec_command(char **args) {
  redirect_t *rd = get_redirection(args);

  if (includes(args, "|")) {
    return piping(args);
  }

  if (is_builtin_command(args[0])) {
    if (rd->filename)
      redirect(rd);

    return exec_builtin_command(args);
  }

  pid_t pid = fork();

  if (pid == 0) {
    // child

    if (rd->filename) 
      redirect(rd);

    execvp(args[0], args);

    fprintf(stderr, "%s: command not found\n", args[0]);
    exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {

    fprintf(stderr, "Failed to fork\n");

    return 1;
  }

  return 0;
}
