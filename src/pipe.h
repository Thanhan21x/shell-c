#ifndef PIPE_H
#define PIPE_H

typedef struct PIPING_T {
  char **left;
  char **right;
} piping_t;

int piping(char **args);


#endif // !PIPE_H
