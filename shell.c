/*******************************************************************************
 * name: shell.c
 * author: godynezz
 * description: shell main function
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "core.h"
#include "errno.h"
#include "signal.h"


void handle_sig(int sig);

unsigned char last_exit_status = 0;
int inputfd;

// i dont know where to go from here

int main(int argc, const char **argv) {
    size_t buffer_size, nread;
    FILE *input_stream;
    char **tokens;
    char *input;

    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            printf("couldn't open file");
            return 1;
        }
    }
    else if (argc > 2) {
        printf("usage: yassh <script path>");
        return 1;
    }
    else {
        input_stream = stdin;
    }

    inputfd = fileno(input_stream);
    if (inputfd == -1) {
        perror("fileno");
        return 1;
    }

    signal(SIGINT, handle_sig);
    setbuf(stdout, NULL);

    while (1) {
        buffer_size = 0;
        input = NULL;
        /* prompt */
        if (isatty(inputfd)) {
            printf(PROMPT);
        }
        if ((nread = getline(&input, &buffer_size, input_stream)) ==
            (size_t)-1) {
            if (errno == EINVAL || errno == ENOMEM) {
                perror("getline");
                return 1;
            }
            else {
                putchar('\n');
                return 0;
            }
        }

        if (nread > 0 && input[nread - 1] == '\n')
            input[nread - 1] = '\0';

        tokens = parse_input(input);
        if (!tokens || !tokens[0]) goto clean;
        if (exec_buildin(tokens)) goto clean;
        if (exec_program(tokens)) goto clean;
        printf("%s: command not found\n", tokens[0]);

    clean:
        free_tokens(tokens);
        free(input);
    }

    if (input_stream != stdin) {
        fclose(input_stream);
    }
    putchar('\n');
    return 0;
}

void handle_sig(int sig) {
    if (sig == SIGINT) {
        printf("\n");
        if (isatty(inputfd)) printf(PROMPT);
    }
}
