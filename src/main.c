#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_NUM 128
#define MAX_COMMAND_LEN 32

int is_builtin_command(const char* cmd) {
  if (!strcmp(cmd, "exit") || 
      !strcmp(cmd, "echo") ||
      !strcmp(cmd, "type")) {
    return 1;
  } else {
    return 0;
  }
}


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

    } else if (strstr(command, "type")) {
      char *cmd = memchr(arg_start, ' ', strlen(arg_start)) 
      if (is_builtin_command(cmd)) {
        printf("%s is a shell builtin\n", cmd);
      } else {
        printf("%s: not found\n", cmd);
      }
    } else {
      printf("%s: command not found\n", command);
    }

  }

  return 0;
}
