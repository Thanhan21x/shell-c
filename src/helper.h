#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sys/wait.h"
#include <dirent.h>
#include <unistd.h>



int is_builtin_command(const char* arg);
int is_executable(const char *path);
int is_executable_in_path(const char* file, const char *path);
int is_executable_command(const char *arg, char *path_out);

void get_arg(const char *cmd, int *argc, char *argv[]);

#endif // !HELPER_H
