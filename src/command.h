#ifndef SHELL_COMMAND_H_INCLUDED
#define SHELL_COMMAND_H_INCLUDED

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Argument_tag {
    int                 index;
    char                *arg;
    struct Argument_tag *next;
} Argument;

typedef struct FDTable_tag {
    FILE               *from;
    FILE               *to;
    struct FDTable_tag *next;
} FDTable;

typedef enum {
    YIELD_NONE,
    YIELD_BACKGROUND,
    YIELD_PIPE
} YieldType;

typedef struct Command_tag {
    char               *name;
    int                argc;
    Argument           *argv;
    FDTable            *fd_table;
    YieldType          yield_type;
    struct Command_tag *next;
} Command;

bool new_command(char *line, Command **c);
void delete_command(Command *c);
void command_execute(Command *c);

#endif
