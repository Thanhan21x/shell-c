#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

parser_t *init_parser(char *src) {
  parser_t *parser = malloc(sizeof(struct PARSER_STRUCT));

  parser->src = src;
  parser->size = strlen(src);
  parser->cursor = 0;
  parser->current = src[0];

  return parser;
}

void parser_advance(parser_t *parser) {
  if (parser->cursor < parser->size && parser->current != '\0') {
    parser->cursor++;
    parser->current = parser->src[parser->cursor];
  }
}

char **parse_input(char *input) {
  parser_t *parser = init_parser(input);
  char **args = malloc(MAX_INPUT * sizeof(char *));
  char token[MAX_INPUT];
  int token_cursor = 0;
  int index =0;

  while (parser->current != '\0') {
    switch (parser->current) {
      case '\r':
      case '\n':
      case ' ':
      case '\t':
        if (token_cursor > 0) {
          token[token_cursor] = '\0';
          args[index++] = strdup(token);
          token_cursor = 0;
        }
        break;
      case '\'':
        parser_advance(parser);
        while (parser->current != '\'') {
          token[token_cursor++] = parser->current;
          parser_advance(parser);
        }
        break;
      case '\"':
        parser_advance(parser);
        while(parser->current != '\"') {
          token[token_cursor++] = parser->current;
          parser_advance(parser);
        }
        break;
      default:
        token[token_cursor++] = parser->current;
        break;
    }
    parser_advance(parser);
  }

  if (token_cursor > 0) {
    token[token_cursor] = '\0';
    args[index++] = strdup(token);
  }

  free(parser);

  if (index == 0) {
    free(args);
    return NULL;
  }

  args[index] = NULL;

  return args;
}
