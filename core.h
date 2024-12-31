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
#define PROMPT "\033[30m~(8:> \033[0m"
#define MAXBUF 2048
#define MAXARG 128

typedef char *string;

typedef struct command {
  string name;
  void (*function)(string *);
} command;

#define ARR_SIZ(arr) sizeof(arr) / sizeof(arr[0])

extern u_int8_t last_exit_status;

// core
bool exec_buildin(string *tokens);
int exec_program(string *tokens);

string get_program_path(string command);
bool is_builtin(string command);

string *parse_input(string input);
void print_arguments(string *tokens);
void free_args(string *args);

// helpers
int is_sig_atoi_able(string str);
int is_unsig_atoi_able(string str);
void TODO();

// builtins commands functions
void shell_exit(string *tokens);
void type(string *tokens);
void echo(string *tokens);
void cls(string *tokens);
void help(string *tokens);
void wdir(string *tokens);
void cd(string *tokens);
void last_status(string *tokens);

#endif
