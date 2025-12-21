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

    // 2D array holding the arguments
    char command_split[32][64];
    int num_arg = 0;
  
    for (int i = 0; i < strlen(command); i++) {
      // for now suppose no starting/trailing spaces,
      // only one space between words
      char arg[64];
      int idx = 0;
      while (command[i] != ' ') {
        arg[idx++] = command[i++];
      }
      arg[idx] = '\0';

      strcpy(command_split[num_arg++], arg);
    }

    num_arg--;

    if (!strcmp(command_split[0], "exit")) {
      break;

    } else if (!strcmp(command_split[0], "echo")) {
      //char *msg = memchr(command, ' ', strlen(command)) + 1;
      printf("%s\n", command_split[1]);

    } else if (!strcmp(command_split[0], "type")) {
      char cmd[64];
      strcpy(cmd, command_split[1]);
      if (is_builtin_command(cmd)) {
        printf("%s is a shell builtin\n", cmd);
      } else {
        printf("%s: not found\n", cmd);
      }
    } else {
      printf("%s: command not found\n", command_split[0]);
    }
  }

  return 0;
}
