/*******************************************************************************
 * name: core.h
 * author: godynezz
 * description: core functions definitions
 ******************************************************************************/

#ifndef CORE_H
#define CORE_H

#define _GNU_SOURCE

#define VERSION "x.x.x"
#define AUTHOR "godynezz"
#define PROMPT "~(8:> "
#define MAXBUF 2048
#define MAXARG 128

typedef struct command {
    char *name;
    void (*function)(char **);
} command;

#define ARR_SIZ(arr) (sizeof(arr) / sizeof(arr[0]))

extern unsigned char last_exit_status;

// core
int exec_buildin(char **tokens);
int exec_program(char **tokens);

char *get_program_path(char *command);
int is_builtin(char *command);

char **parse_input(char *input);
void free_args(char **args);

// validation
int is_sig_atoi_able(char *str);
int is_unsig_atoi_able(char *str);

// debug
void TODO(void);
void print_arguments(char **tokens);

// builtins commands
void shell_exit(char **tokens);
void type(char **tokens);
void echo(char **tokens);
void cls(char **tokens);
void help(char **tokens);
void wdir(char **tokens);
void cd(char **tokens);
void last_status(char **tokens);

#endif
