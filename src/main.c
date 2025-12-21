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

    char input[1024];
    fgets(input, sizeof(input), stdin);

    // Remove the trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Take the command which is the word before the first space
    char command[64];
    int command_len = 0;
    for (int i = 0; i < strlen(input); i++) {
      if (input[i] == ' ') {
        break;
      } else {
        command[command_len++] = input[i];
      }
    }
    command[command_len] = '\0';

    if (!strcmp(command, "exit")) {
      break;

    } else if (!strcmp(command, "echo")) {
      char *msg = memchr(input, ' ', strlen(input)) + 1;
      printf("%s\n", msg);

    } else if (!strcmp(command, "type")) {
      char *cmd = memchr(input, ' ', strlen(input)) + 1;
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
