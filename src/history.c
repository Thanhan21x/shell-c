#include "history.h"

#include <stdio.h>
#include <readline/history.h>
#include <string.h>


void add_history_from_file(char *path) {
  FILE* fp = fopen(path, "rb");

  char *line = NULL;
  size_t cap = 0;

  while (getline(&line, &cap, fp) != -1) {
    line[strcspn(line, "\n")] = '\0';
    add_history(line);
  }

  fclose(fp);
}

void write_history_to_file(char *path) {
  FILE *fp = fopen(path, "w");

  HIST_ENTRY **hist = history_list();

  for (int i = 0; hist[i]; i++) {
    fprintf(fp, "%s\n", hist[i]->line);
  }


  fclose(fp);
}

void append_history_to_file(char *path) {
  HIST_ENTRY **hist = history_list();

  FILE* fp = fopen(path, "a");

  static int i = 0;
  for (i; hist[i]; i++) {
    fprintf(fp, "%s\n", hist[i]->line);
  }

  fclose(fp);
}
