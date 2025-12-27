#include "redirect.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

redirect_t *is_redirection(char **args) {
  redirect_t *rd = malloc(sizeof(redirect_t));
  for (int i = 0; args[i] != NULL; i++) {
    if (!strcmp(args[i], ">") || !strcmp(args[i], "1>")) {
      rd->filename = strdup(args[i+1]);
      rd->type = 1;
      args[i] = NULL;
      args[i+1] = NULL;
      return rd;
    } else if (!strcmp(args[i], "2>")) {
      rd->filename = strdup(args[i+1]);
      rd->type = 2;
      args[i] = NULL;
      args[i+1] = NULL;
      return rd;
    } 
  }

  rd->filename = NULL;
  rd->type = -1;
  return rd;

}



