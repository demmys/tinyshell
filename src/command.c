#include "command.h"

bool new_command(Command **c) {
    static const char dlm[3] = " \t";

    Argument *a;
    Buffer *b;
    char ch, *line, *arg;
    int argc;

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
    (*c)->argv = NULL;

    for(argc = 0; true; ++argc) {
        arg = strtok(NULL, dlm);
        if(!arg) {
            break;
        }
        if((*c)->argv) {
            a->next = (Argument *)malloc(sizeof(Argument));
            a = a->next;
        } else {
            (*c)->argv = (Argument *)malloc(sizeof(Argument));
            a = (*c)->argv;
        }
        if(!a) {
            delete_command(*c);
            return false;
        }
        a->arg = arg;
        a->next = NULL;
    }
    (*c)->argc = argc;
    return true;
}

void delete_command(Command *c) {
    Argument *a, *b;
    a = c->argv;
    free(c->name);
    free(c);
    while(a) {
        b = a->next;
        free(a);
        a = b;
    }
}

void command_execute(Command *c) {
    Argument *a;
    char **argv = (char **)malloc(sizeof(char *) * (c->argc + 2));
    int i;

    argv[0] = c->name;
    a = c->argv;
    for(i = 1; a; ++i) {
        argv[i] = a->arg;
        a = a->next;
    }
    argv[i] = NULL;

    execvp(c->name, argv);
    // to free garbage which remained when exec failed
    free(argv);
}
