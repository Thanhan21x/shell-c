#include "completer.h"
#include <string.h>
#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>

extern const char *builtins[]; // from command.c
extern char *xmalloc PARAMS((size_t));
//

char *dupstr(const char *s) {
  char *r;

  r = xmalloc(strlen(s) + 1);
  strcpy(r, s);
  return (r);
}

/* Tell the GNU Readline libary how to complete */
void initialize_readline() {
  rl_readline_name = "SHELL-C";
  
  /* Tell the competer that we want a crack first. */
  rl_attempted_completion_function = shell_completion;
}

/* Attempt to complete on the contents of TEXT. START and END show the
* region of TEXT that contains the word to complete. We can use the 
* entire line in case we want to do some simple parsing. Return the 
* array of matches, or NULL if there aren't any. */
char **shell_completion(const char *text, int start, int end) {
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command 
  * to complete. Other wise it is the name of a file in the current 
  * directory.
  */
  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);

    return (matches);
  }
}

/* Generator function for command completion. STATE lets us know whether
* to start from scratch; without any state (i.e. STATE == 0), then we 
* start at the top of the list.
*/
char *command_generator(const char *text, int state) {
  static int list_index, len;
  const char *name;

  /* If this is a new word to complete, initialze now. This includes 
  * saving the length of TEXT for efficiency, and initializing the index 
  * variable to 0.
  */
  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  /* Return the next name which partially matches from the command list */
  while (name = builtins[list_index]) {
    list_index++;

    if (strncmp(name, text, len) == 0) {
      return dupstr(name);
    }
  }
  /* If no names matched, then return NULL */
  return ((char *)NULL);
}

