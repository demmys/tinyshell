#include "command.h"

bool new_command(Command **c) {
    static const char dlm[3] = " \t";

    Command *d;
    Buffer *b;
    char ch, *line, *arg;

    if(!new_buffer(512, &b)) {
        return false;
    }

    for(ch = getchar(); ch != '\n'; ch = getchar()) {
        buffer_put(b, ch);
    }
    line = buffer_restore(b);
    delete_buffer(b);

    *c = NULL;
    for(arg = strtok(line, dlm); arg; arg = strtok(NULL, dlm)) {
        if(*c) {
            d->next = (Command *)malloc(sizeof(Command));
            d = d->next;
        } else {
            *c = (Command *)malloc(sizeof(Command));
            d = *c;
        }
        if(!d) {
            return false;
        }
        d->contents = arg;
        d->next = NULL;
    }
    return true;
}

void delete_command(Command *c) {
    Command *d;
    free(c->contents);
    while(c) {
        d = c->next;
        free(c);
        c = d;
    }
}

int command_execute(Command *c) {
    while(c) {
        printf("|%s|\n", c->contents);
        c = c->next;
    }
    return 0;
}
