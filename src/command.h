#ifndef SHELL_COMMAND_H_INCLUDED
#define SHELL_COMMAND_H_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Argument_tag {
    int                 index;
    char                *arg;
    struct Argument_tag *next;
} Argument;

typedef struct Command_tag {
    char               *name;
    int                argc;
    Argument           *argv;
    int                input;
    int                output;
    bool               pipe;
    bool               background;
    struct Command_tag *next;
} Command;

bool new_command(char *line, Command **c);
void delete_command(Command *c);
void command_execute(Command *c);

#endif
