#include "command.h"

bool new_command(Command **c) {
    static const char dlm[3] = " \t";

    Argument *a;
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

    arg = strtok(line, dlm);
    if(!arg) {
        return false;
    }
    *c = (Command *)malloc(sizeof(Command));
    (*c)->name = arg;
    (*c)->arguments = NULL;

    while(true) {
        arg = strtok(NULL, dlm);
        if(!arg) {
            break;
        }
        if((*c)->arguments) {
            a->next = (Argument *)malloc(sizeof(Argument));
            a = a->next;
        } else {
            (*c)->arguments = (Argument *)malloc(sizeof(Argument));
            a = (*c)->arguments;
        }
        if(!a) {
            delete_command(*c);
            return false;
        }
        a->arg = arg;
        a->next = NULL;
    }
    return true;
}

void delete_command(Command *c) {
    Argument *a, *b;
    a = c->arguments;
    free(c->name);
    free(c);
    while(a) {
        b = a->next;
        free(a);
        a = b;
    }
}

int command_execute(Command *c) {
    Argument *a = c->arguments;
    printf("|%s|\n", c->name);
    while(a) {
        printf("|%s|\n", a->arg);
        a = a->next;
    }
    return 0;
}
