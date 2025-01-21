/*******************************************************************************
 * name: shell.c
 * author: godynezz
 * description: shell main function
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "errno.h"

unsigned char last_exit_status = 0;

// TODO:
// if Ctrl+c signal is sent the shell exits instead of stoping the child
// process and return to the prompt

// TODO:
// parse_input: implement with variable size  arrays so MAXBUF and MAXARG are
// not needed

int main(void) {
    size_t buffer_size, nread;
    char** tokens;
    char* input;

    setbuf(stdout, NULL);

    while (1) {
        buffer_size = 0;
        input = NULL;
        /* prompt */
        printf(PROMPT);
        if ((nread = getline(&input, &buffer_size, stdin)) == (size_t)-1) {
            if (errno == EINVAL || errno == ENOMEM) {
                perror("getline");
                return 1;
            } else {
                putchar('\n');
                return 0;
            }
        }
        input[nread - 1] = '\0';
        tokens = parse_input(input);

        if (!tokens || !tokens[0]) goto clean;
        if (exec_buildin(tokens)) goto clean;
        if (exec_program(tokens)) goto clean;
        printf("%s: command not found\n", tokens[0]);

    clean:
        free_args(tokens);
        free(input);
    }

    putchar('\n');
    return 0;
}
