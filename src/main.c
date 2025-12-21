#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  
  char command[1024];
  while (1) {
    printf("$ ");

    fgets(command, sizeof(command), stdin);

    // Remove the trailing newline
    command[strcspn(command, "\n")] = '\0';

    if (!strcmp(command, "exit")) {
      break;

    } else if (!strcmp(command, "echo")) {
      printf("%s\n", command);

    } else {
      printf("%s: command not found\n", command);
    }

  }

  return 0;
}
