#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_COMMAND_NUM 128
#define MAX_COMMAND_LEN 32


int is_builtin_command(const char *arg) {
  if (!strcmp(arg, "exit") || 
      !strcmp(arg, "echo") ||
      !strcmp(arg, "type")) {
    return 1;
  } else {
    return 0;
  }
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

  char *paths = getenv("PATH");

  char *end = memchr(paths, '\0', strlen(paths) + 1);
  if (end == NULL) {
    fprintf(stderr,"Wrong string format for PATH");
    exit(1);
  }

  while (paths < end) {
    char *colon = memchr(paths, ':', strlen(paths) + 1);

    char path[256];
    if (colon) {
      int path_len = colon - paths;
      memcpy(path, paths, path_len);
      path[path_len] = '\0';
      if (is_executable_in_path(arg, path)) {
        strcpy(path_out, path);
        return 1;
      }
      paths += strlen(path) + 1;
    } else {
      strcpy(path, paths);
      if (is_executable_in_path(arg, path)) {
        strcpy(path_out, path);
        return 1;
      }
      paths += strlen(path) + 1 ;
    }
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

      printf("arg %d: %s\n", count++, arg);



      if (*p == '\0')
        break;

      start = p + 1;
    }
    p++;
  }

  *argc = count;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");

    char path_found[64];

    int _argc;
    char *_argv[1024];

    char input[1024];
    fgets(input, sizeof(input), stdin);
 
    // Remove the trailing newline
    input[strcspn(input, "\n")] = '\0';
 
    get_arg(input, &_argc, _argv);
    printf("argument count: %d\n", _argc);


    // Take the command which is the word before the first space
    char command[64];
    char *space_pos = memchr(input, ' ', strlen(input));


    if (space_pos == NULL) {
      // only one argument
      strcpy(command, input);
    } else {
      int len = space_pos - input;
      memcpy(command, input, len);
      command[len] = '\0';
    }
    
    // Command exit
    if (!strcmp(command, "exit")) {
      break;

    // Command echo
    } else if (!strcmp(command, "echo")) {
      char *msg = memchr(input, ' ', strlen(input)) + 1;
      printf("%s\n", msg);

    // Command type
    } else if (!strcmp(command, "type")) {
      char *arg = memchr(input, ' ', strlen(input)) + 1;
      // Check if it's a builtin command
      if (is_builtin_command(arg)) {
        printf("%s is a shell builtin\n", arg);
      // Check if it's an executable, print its path if found
      } else if (is_executable_command(arg, path_found)) {
        // copy the executable's path to path
        printf("%s is %s/%s\n", arg, path_found, arg);
      } else {
        printf("%s: not found\n", arg);
      }

    // Run Executable
    } else if (is_executable_command(command, path_found)) {
      continue;

    // Invalid command
    } else {
      printf("%s: command not found\n", command);
    }
  }

  return 0;
}
