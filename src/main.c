#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sys/wait.h"
#include <dirent.h>
#include <unistd.h>

#define MAX_COMMAND_NUM 128
#define MAX_COMMAND_LEN 32

const char shell_builtin[] = "exit:echo:type:pwd";


int is_builtin_command(const char* arg) {
  char *cmds = strdup(shell_builtin);

  char *cmd = strtok(cmds, ":");

  while(cmd) {
    if (!strcmp(arg, cmd)) {
      return 1;
    }

    cmd = strtok(NULL, ":");
  }
 
  return 0;
}

int is_executable(const char *path) {
  struct stat st;

  if (stat(path, &st) != 0)
    return 0;

  return (st.st_mode & S_IXUSR) != 0;
}

int is_executable_in_path(const char* file, const char *path) {
  // list all executable in path and compare it with the arg
  DIR *dirp = opendir(path); 
  if (!dirp) {
    perror("opendir");
    exit(1);
  }
  struct dirent *entry;

  while (entry = readdir(dirp)) {
    if (!strcmp(entry->d_name, file)) {
      char full_path[256];
      snprintf(full_path, sizeof(full_path), "%s/%s", path, file);
      if (is_executable(full_path)){
        return 1;
      }
    }
  }

  return 0;
}

int is_executable_command(const char *arg, char *path_out) {

  char *paths = strdup(getenv("PATH"));

  char *path = strtok(paths, ":");

  while (path) {
    if (is_executable_in_path(arg, path)) {
      strcpy(path_out, path);
      return 1;
    }

    path = strtok(NULL, ":");
  }

  return 0;
}

void get_arg(const char *cmd, int *argc, char *argv[]) {
  const char *start = cmd;
  const char *p = cmd;
 
  int count = 0;

  while (1) {
    if (*p == ' ' || *p == '\0') {
      char arg[32];

      memcpy(arg, start, (int)(p - start));
      arg[p-start] = '\0';

      //printf("arg %d: %s\n", count, arg);
      argv[count++] = strdup(arg);

      if (*p == '\0')
        break;

      start = p + 1;
    }
    p++;
  }
  
  argv[count] = NULL;

  *argc = count;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");

    char exec_path[64];

    int _argc;
    char *_argv[1024];

    char input[1024];
    fgets(input, sizeof(input), stdin);
 
    // Remove the trailing newline
    input[strcspn(input, "\n")] = '\0';

 
    get_arg(input, &_argc, _argv);

    // Take the command which is the word before the first space
    char command[64];
    strcpy(command, _argv[0]);

    //printf("command: %s\n", command);

    if (strlen(command) < 1) {
      continue;

    // Command exit
    } else if (!strcmp(command, "exit")) {
      break;

    // Command echo
    } else if (!strcmp(command, "echo")) {
      // echo everything after the space
      char *msg = memchr(input, ' ', strlen(input)) + 1;
      printf("%s\n", msg);

    // Command type
    } else if (!strcmp(command, "type")) {
      
      //printf("argc: %d\n", argc);
      for (int i = 1; i < _argc; i++) {
        if (is_builtin_command(_argv[i])) {
          printf("%s is a shell builtin\n", _argv[i]);
        // Check if it's an executable, print its path if found
        } else if (is_executable_command(_argv[i], exec_path)) {
          // copy the executable's path to path
          printf("%s is %s/%s\n", _argv[i], exec_path, _argv[i]);
        } else {
          printf("%s: not found\n", _argv[i]);
        }
      }

    // Run Executable
    } else if (!strcmp(command, "pwd")) {
      char cwd[256];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);

    } else if (is_executable_command(command, exec_path)) {
      char full_path[128];
      snprintf(full_path, sizeof(full_path), "%s/%s", exec_path, command);

      //printf("full_path: %s\n", full_path);

      pid_t pid = fork();

      // in child process
      if (pid == 0) {
        execv(full_path, _argv);
        perror("execv");
        _exit(1);
      // in parent process (this program)
      } else if (pid > 0) {
        // wait for the chil to exit
        waitpid(pid, NULL, 0); // wait for child process done
      } else {
        perror("fork");
      }

    } else {
      printf("%s: command not found\n", command);
    }
  }

  return 0;
}
