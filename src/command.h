#ifndef SHELL_COMMAND_H_INCLUDED
#define SHELL_COMMAND_H_INCLUDED

#include <stdio.h>
#include "buffer.h"

typedef struct Command_tag {
    char *contents;
    struct Command_tag *next;
} Command;

Command *new_command();
void delete_command(Command *c);

#endif
