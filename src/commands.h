#ifndef COMMANDS_H
#define COMMANDS_H

#include "helper.h"

void command_pwd();

void command_cd(char **args);

void command_type(char** args);

void command_echo(char **args);

void command_run(char **args, char *path);



#endif // !COMMANDS_H
