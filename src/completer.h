#ifndef COMPLETER_H
#define COMPLETER_H

char *command_generator(const char *text, int state);

char **shell_completion(const char *text, int start, int end);

void initialize_readline();

char *dupstr(const char *);

typedef char **CPPFunction();

#endif // !COMPLETER_H
