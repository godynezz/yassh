/*******************************************************************************
 * name: core.c
 * author: godynezz
 * description: core functions
 ******************************************************************************/
#include "core.h"

static string HELP_MESSAGE =
    "\nversion: %s\n"
    "type [command] - print the type of command\n"
    "echo [string]  - print everything you pass to it\n"
    "exit [n]       - exit the shell with status n\n"
    "cls            - clear the screen\n"
    "help [?opt]    - print this help message opt can be\n"
    "                `about` or just leave blank \n"
    "pwd            - print working directory\n"
    "?              - print the last exit status\n"
    "cd [?dir]      - change working directory to dir if you leave dir\n"
    "                blank it will change to home\n\n";

static string ABOUT =
    "\nyassh (Yet Another Stupid Shell) is a simple shell made for fun\n"
    "and learning purposes. It is not secure and I don't plan to make it be,\n"
    "Do not use in production! \n\n"
    "This shell can handle basic stuff, builtins and external programs\n"
    "on system PATH and relative to working directory, and the most important\n"
    "the prompt is a mouse. ~(8:>\n"
    "author: %s\n\n";

static command builtins[] = {
    {"pwd", &wdir},  {"cd", &cd},   {"exit", &shell_exit}, {"type", &type},
    {"echo", &echo}, {"cls", &cls}, {"help", &help},       {"?", &last_status}};

bool is_builtin(string command) {
  for (size_t index = 0; index < ARR_SIZ(builtins); index++) {
    if (!strcmp(command, builtins[index].name)) return true;
  }
  return false;
}

string get_program_path(string command) {
  string path, path_copy, dir, program_path;

  program_path = malloc(MAXBUF);
  if (!program_path) {
    perror("malloc");
    return NULL;
  }

  path = getenv("PATH");
  if (!path) {
    free(program_path);
    return NULL;
  }

  path_copy = strdup(path);
  if (!path_copy) {
    perror("strdup");
    free(program_path);
    return NULL;
  }

  dir = strtok(path_copy, ":");
  while (dir) {
    snprintf(program_path, MAXBUF, "%s/%s", dir, command);
    if (!access(program_path, X_OK)) {
      free(path_copy);
      return program_path;
    }
    dir = strtok(NULL, ":");
  }

  free(path_copy);
  free(program_path);
  return NULL;
}

void TODO() { printf("to be implemented\n"); }

int is_sig_atoi_able(string str) {
  if (*str == '-') str++;
  if (!*str) return false;

  while (*str) {
    if (!isdigit(*str))
      return false;
    else
      str++;
  }
  return true;
}

int is_unsig_atoi_able(string str) {
  if (!*str) return false;

  while (*str) {
    if (!isdigit(*str))
      return false;
    else
      str++;
  }
  return true;
}

void print_arguments(string *tokens) {
  for (int i = 0; tokens[i]; i++) {
    printf("%s ", tokens[i]);
  }
  printf("\n");
}

void shell_exit(string *tokens) {
  int exit_status = 0;
  if (tokens[2]) {
    printf("too many arguments\n");
    return;
  }

  if (tokens[1]) {
    if (!is_unsig_atoi_able(tokens[1])) {
      printf("[n] should be an unsigned int\n");
      return;
    }
    exit_status = atoi(tokens[1]);
  }
  exit(exit_status);
}

void type(string *tokens) {
  if (tokens[2]) {
    printf("too many arguments\n");
    return;
  }

  if (is_builtin(tokens[1])) {
    printf("%s is a shell builtin\n", tokens[1]);
    return;
  }

  string tmp = get_program_path(tokens[1]);
  if (tmp) {
    printf("%s is %s\n", tokens[1], tmp);
    free(tmp);
    return;
  }
  printf("command not found: %s\n", tokens[1]);
}

void echo(string *tokens) { print_arguments(tokens + 1); }

void cls(string *tokens) {
  if (tokens[1]) {
    printf("too many arguments\n");
    return;
  }
  printf("\033[H\033[J");
}

void help(string *tokens) {
  if (tokens[1]) {
    if (!strcmp(tokens[1], "about")) {
      printf(ABOUT, AUTHOR);
      return;
    }
  } else
    printf(HELP_MESSAGE, VERSION);
}

void wdir(string *tokens) {
  char cwd[BUFSIZ];

  if (tokens[1]) {
    printf("too many arguments\n");
    return;
  }
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    puts(cwd);
  } else {
    perror("getcwd() error");
    return;
  }
}

void cd(string *tokens) {
  char pathbuf[BUFSIZ];
  if (!tokens[1]) {
    string home = getenv("HOME");
    if (!home) {
      fprintf(stderr, "HOME environment variable not set\n");
      last_exit_status = 1;
      return;
    }
    snprintf(pathbuf, BUFSIZ, "%s", home);

  } else if (tokens[1][0] == '~') {
    string home = getenv("HOME");
    if (!home) {
      fprintf(stderr, "HOME environment variable not set\n");
      last_exit_status = 1;
      return;
    }
    snprintf(pathbuf, BUFSIZ, "%s/%s", home, tokens[1] + 1);
  } else {
    snprintf(pathbuf, BUFSIZ, "%s", tokens[1]);
  }

  if (chdir(pathbuf) != 0) {
    perror("cd");
    last_exit_status = 1;
  } else {
    last_exit_status = 0;
  }
}

void last_status(string *tokens) {
  if (tokens[1]) {
    printf("too many arguments\n");
    return;
  }
  printf("status code: %d\n", last_exit_status);
}

int exec_program(string *tokens) {
  // search for program in PATH
  string program_path = get_program_path(tokens[0]);

  if (program_path) {
    // make exec_program return an int and set last_exit_status
    free(tokens[0]);
    tokens[0] = program_path;
  } else if (!access(tokens[0], X_OK) == 0) {
    return false;
  }

  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) execv(tokens[0], tokens);
  if (pid < 0) {
    perror("fork");
  } else {
    waitpid(pid, &status, 0);
    last_exit_status = WEXITSTATUS(status);
  }

  return true;
}

bool exec_buildin(string *tokens) {
  for (size_t index = 0; index < ARR_SIZ(builtins); index++) {
    if (!strcmp(tokens[0], builtins[index].name)) {
      builtins[index].function(tokens);
      return true;
    }
  }
  return false;
}

string *parse_input(string input) {
  string *tokens = malloc(sizeof(string) * MAXARG);
  char tmp[MAXBUF];
  char cur_char;
  int index;
  int argindex;

  for (argindex = index = 0;
       (cur_char = input[index]) && argindex < (MAXARG - 1);
       index++, argindex++, memset(tmp, 0, MAXBUF)) {
    if (isspace(cur_char)) {
      while (isspace(cur_char = input[index])) {
        index++;
      }
    }

    if (cur_char == '"') {
      index++;
      int tmp_index = 0;
      for (; (cur_char = input[index]) != '\0' && cur_char != '"' &&
             tmp_index < MAXBUF;
           index++) {
        tmp[tmp_index++] = cur_char;
      }
      index++;
      tmp[tmp_index++] = '\0';
      if (cur_char != '"') {
        printf("shell error: syntax error\n");
        return NULL;
      }
      tokens[argindex] = strdup(tmp);
      continue;
    }

    if (cur_char == '\'') {
      index++;
      int tmp_index = 0;
      for (; (cur_char = input[index]) != '\0' && cur_char != '\'' &&
             (tmp_index) < MAXBUF;
           index++) {
        tmp[tmp_index++] = cur_char;
      }
      index++;
      tmp[tmp_index++] = '\0';
      if (cur_char != '\'') {
        printf("shell error: syntax error\n");
        return NULL;
      }
      tokens[argindex] = strdup(tmp);
      continue;
    }

    if (!isspace(cur_char)) {
      int tmp_index = 0;
      while (!isspace(cur_char = input[index]) && cur_char != '\0' &&
             tmp_index < MAXBUF) {
        if (cur_char == '\\' && input[index + 1] != '\0') {
          cur_char = input[++index];
        }
        tmp[tmp_index++] = cur_char;
        index++;
      }
      tmp[tmp_index] = '\0';
      tokens[argindex] = strdup(tmp);
      continue;
    }
  }
  tokens[argindex] = NULL;
  return tokens;
}

void free_args(string *args) {
  if (!args) return;

  for (int i = 0; args[i]; i++) {
    free(args[i]);
  }
  free(args);
}
