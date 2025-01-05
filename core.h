/*******************************************************************************
 * name: core.c
 * author: godynezz
 * description: core functions definitions
 ******************************************************************************/

#ifndef CORE_H
#define CORE_H

#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define VERSION "x.x.x"
#define AUTHOR "godynezz"
#define PROMPT "~(8:> "
#define MAXBUF 2048
#define MAXARG 128

struct command {
  char *name;
  void (*function)(char **);
} command;

#define ARR_SIZ(arr) sizeof(arr) / sizeof(arr[0])

extern u_int8_t last_exit_status;

// core
bool exec_buildin(char **tokens);
int exec_program(char **tokens);

char *get_program_path(char *command);
bool is_builtin(char *command);

char **parse_input(char *input);
void free_args(char **args);

// helpers
int is_sig_atoi_able(char *str);
int is_unsig_atoi_able(char *str);

// debug
void TODO(void);
void print_arguments(char **tokens);

// builtins commands functions
void shell_exit(char **tokens);
void type(char **tokens);
void echo(char **tokens);
void cls(char **tokens);
void help(char **tokens);
void wdir(char **tokens);
void cd(char **tokens);
void last_status(char **tokens);

#endif
