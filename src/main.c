#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  
  while (1) {
    printf("$ ");

    char command[1024];
    fgets(command, sizeof(command), stdin);

    // Remove the trailing newline
    command[strcspn(command, "\n")] = '\0';

    if (!strcmp(command, "exit")) {
      break;

    } else if (strstr(command, "echo")) {
      char *msg = memchr(command, ' ', strlen(command)) + 1;
      printf("%s\n", msg);

    } else {
      printf("%s: command not found\n", command);
    }

  }

  return 0;
}
