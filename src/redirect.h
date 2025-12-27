#ifndef REDIRECT_H
#define REDIRECT_H

/*
 * This module is for the redirection.
 *
 * If the command prompt contain the redirect > 
 *  1 -> redirect the stdout
 *  2 -> reditect the stderr
 */

typedef struct REDIRECT_T{
  char *filename;
  int type;
} redirect_t;

redirect_t *is_redirection(char **args);

#endif // !REDIRECT_H
