#ifndef COMMANDS_H
#include <stdbool.h>
#include <stdio.h>

#define COMMANDS_H

bool is_builtin_command(char *command);

int exec_builtin_cd(char **args, FILE *file);

int exec_builtin_echo(char **args, FILE *file);

int exec_builtin_exit(char **args);

int exec_builtin_pwd(FILE *file);

int exec_builtin_type(char **args, FILE *file);

int exec_builtin_command(char **args, FILE *file);

int exec_command(char **args, FILE *ouput);
#endif
