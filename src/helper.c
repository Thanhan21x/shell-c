#include "helper.h"

const char shell_builtin[] = "exit:echo:type:pwd:cd";

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

/*
void get_arg(const char *cmd, int *argc, char *argv[]) {
  const char *start = cmd;
  const char *p = cmd;
 
  int count = 0;

  while (1) {
    // case: no single quote
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
*/

void get_arg(const char *cmd, int *argc, char *argv[]) {
  const char *start = cmd; // start of an arg
  const char *p = cmd; // pointer to the current char
  //


  // Iterate over the cmd
  while (1) {
    // ignore empty quote
    if (*p == '\'' &&p[1] != '\0' && *(p+1) == '\'') {
      p += 2;

    // encounter openning single quote
    } else if (*p == '\'' && *(p+1) != '\'') {
      p++; // start char is right after p

      char arg[32];
      int idx = 0;
      // while not encounter 
      while (*p != '\'' && *p != '\0') {
        arg[idx++] = *(p++);
        // ignore inner empty quote
        if (*p == '\'' && *(p+1) == '\'') {
          p += 2;
        }
      }
      // p now points to the char after the ending quote
      arg[idx] = '\0';

      argv[(*argc)++] = strdup(arg);

      while (*p == ' ') {
        // ignore trailing space
        p++;
      }
      start = p;

      printf("arg: %s\n", arg);
    // regular arg (separate by spaces)
    } else if (*p == ' ' || *p == '\0') {
      char arg[32];

      memcpy(arg, start, (int)(p - start));
      arg[p-start] = '\0';

      argv[(*argc)++] = strdup(arg);

      if (*p == '\0')
        break;

      start = p++;

      printf("arg: %s\n", arg);

    } else {
      p++;
    }
  }

  // add NULL terminal
  argv[*argc] = NULL;
}
