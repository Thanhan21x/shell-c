#include "commands.h"
#include "path_builder.h"
#include "sys/wait.h"
#include "utils.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *builtins[] = {"cd", "echo", "exit", "pwd", "type", NULL};

bool is_builtin_command(char *command) {
  return includes((char **)builtins, command);
}

int exec_builtin_cd(char **args, FILE *file) {
  char *path = build_path(args[1], file);

  if (path == NULL) {
    return 1;
  }

  int code = chdir(path);
  if (code != 0) {
    fprintf(file, "cd: %s: No such file or directory\n", path);
  }

  free(path);

  return code;
}

int exec_builtin_echo(char **args, FILE *file) {
  char *output = str_join_from(args, 1, " ");
  printf("output echo: %d\n", output);
  fprintf(file, "%s\n", output);

  free(output);

  return 0;
}

int exec_builtin_exit(char **args) {
  int exit_code = args[1] != NULL ? atoi(args[1]) : 0;

  cleanup_args(args);

  exit(exit_code);
}

int exec_builtin_pwd(FILE *file) {
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer)) != NULL) {
    fprintf(file, "%s\n", buffer);

    return 0;
  }

  fprintf(file, "Could not get current working directory");

  return 1;
}

int exec_builtin_type(char **args, FILE *file) {
  if (args[1] == NULL) {
    return 1;
  }

  if (is_builtin_command(args[1])) {
    fprintf(file, "%s is a shell builtin\n", args[1]);

    return 0;
  }

  char *paths = strdup(getenv("PATH"));

  if (paths == NULL) {
    fprintf(file, "%s: not found\n", args[1]);

    return 1;
  }

  char *path = strtok(paths, ":");

  while (path != NULL) {
    char fullpath[PATH_MAX];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, args[1]);

    if (access(fullpath, X_OK) == 0) {
      fprintf(file, "%s is %s\n", args[1], fullpath);

      free(paths);

      return 0;
    }

    path = strtok(NULL, ":");
  }

  free(paths);

  fprintf(file, "%s: not found\n", args[1]);

  return 1;
}

int exec_builtin_command(char **args, FILE *file) {
  if (strcmp(args[0], "cd") == 0) {
    return exec_builtin_cd(args, file);
  }

  if (strcmp(args[0], "echo") == 0) {
    return exec_builtin_echo(args, file);
  }

  if (strcmp(args[0], "exit") == 0) {
    return exec_builtin_exit(args);
  }

  if (strcmp(args[0], "pwd") == 0) {
    return exec_builtin_pwd(file);
  }

  if (strcmp(args[0], "type") == 0) {
    return exec_builtin_type(args, file);
  }

  return 1;
}

int exec_command(char **args, FILE *file) {
  if (is_builtin_command(args[0])) {
    return exec_builtin_command(args, file);
  }

  pid_t pid = fork();

  if (pid == 0) {
    execvp(args[0], args);

    fprintf(file, "%s: command not found\n", args[0]);
    exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    fprintf(file, "Failed to fork\n");

    return 1;
  }

  return 0;
}
