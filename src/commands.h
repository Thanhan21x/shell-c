#ifndef COMMANDS_H
#define COMMANDS_H

#include "helper.h"

void command_type(int _argc, char* _argv[]);

void command_run(char *path, char *cmd, char *_argv[]);

void command_cd(char *path);


#endif // !COMMANDS_H
