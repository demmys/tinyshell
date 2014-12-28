#ifndef SHELL_COMMAND_H_INCLUDED
#define SHELL_COMMAND_H_INCLUDED

#include <stdio.h>
#include <unistd.h>
#include "buffer.h"

typedef struct Argument_tag {
    char *arg;
    struct Argument_tag *next;
} Argument;

typedef struct Command_tag {
    char *name;
    int argc;
    Argument *argv;
} Command;

bool new_command(Command **c);
void delete_command(Command *c);
void command_execute(Command *c);

#endif
