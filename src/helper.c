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
