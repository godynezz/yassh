/*******************************************************************************
 * name: shell.c
 * author: godynezz
 * description: shell main function
 ******************************************************************************/

#include "core.h"

u_int8_t last_exit_status = 0;

int main(void) {
  setbuf(stdout, NULL);
  while (true) {
    char input[MAXBUF];
    
    // prompt
    printf(PROMPT);
    if (!fgets(input, MAXBUF, stdin)) break;
    input[strlen(input) - 1] = '\0';  // remove newline

    char ** tokens = parse_input(input);
    
    if (!tokens || !tokens[0]) {
      free_args(tokens);
      continue;
    }

    // execute built-in command
    if (exec_buildin(tokens)) {
      free_args(tokens);
      continue;
    }

    // execute program
    // check if the program is in the path and executes it
    // if not on path check if it is relative to the current directory
    if(exec_program(tokens)) {
      free_args(tokens);
      continue;
    }

    // if anything was found
    printf("%s: command not found\n", tokens[0]);
    free_args(tokens);
  }

  putchar('\n');
  return 0;
}
