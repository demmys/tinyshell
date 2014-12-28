#ifndef SHELL_COMMAND_H_INCLUDED
#define SHELL_COMMAND_H_INCLUDED

#include <stdio.h>
#include "buffer.h"

typedef struct Command_tag {
    char *contents;
    struct Command_tag *next;
} Command;

bool new_command(Command **c);
void delete_command(Command *c);
int command_execute(Command *c);

#endif
