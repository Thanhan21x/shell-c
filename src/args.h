#ifndef ARGS_H
#define ARGS_H

#include <stddef.h>

#define MAX_INPUT 1024

typedef struct PARSER_STRUCT {
  char *src;
  size_t size;
  char current;
  int cursor;
} parser_t;

parser_t *init_parser(char *src);

void parser_advance(parser_t *parser);

char **parse_input(char *input);

#endif
