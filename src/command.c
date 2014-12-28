#include "command.h"

static const char _DLM[3] = " \t";

bool _new_fd_table(FILE *from, FILE *to, Command *c) {
    FDTable **f = &(c->fd_table);
    while(*f) {
        f = &((*f)->next);
    }
    *f = (FDTable *)malloc(sizeof(FDTable));
    if(!*f) {
        return false;
    }
    (*f)->from = from;
    (*f)->to = to;
    (*f)->next = NULL;
    return true;
}

bool _command_malloc(char *name, Command **c) {
    Command *d;
    *c = (Command *)malloc(sizeof(Command));
    if(!*c) {
        return false;
    }
    d = *c;
    d->name = name;
    d->argc = 0;
    d->argv = NULL;
    d->fd_table = NULL;
    d->yield_type = YIELD_NONE;
    d->next = NULL;
    return true;
}

char *_get_next_token(char *token) {
    if(token[1] == '\0') {
        return strtok(NULL, _DLM);
    } else {
        return token + 1;
    }
}

bool _command_create(char *name, Command **c) {
    Argument *a;
    char *token;
    FILE *f;

    if(!_command_malloc(name, c)) {
        return false;
    }
    while(true) {
        token = strtok(NULL, _DLM);
        if(!token) {
            break;
        }
        switch(token[0]) {
            case '<':
                token = _get_next_token(token);
                if(!token) {
                    delete_command(*c);
                    return false;
                }
                f = fopen(token, "r");
                if(!f || !_new_fd_table(stdin, f, *c)) {
                    delete_command(*c);
                    return false;
                }
                break;
            case '>':
                token = _get_next_token(token);
                if(!token) {
                    delete_command(*c);
                    return false;
                }
                f = fopen(token, "w");
                if(!f || !_new_fd_table(stdout, f, *c)) {
                    delete_command(*c);
                    return false;
                }
                break;
            case '|':
                (*c)->yield_type = YIELD_PIPE;
                token = _get_next_token(token);
                if(!token) {
                    delete_command(*c);
                    return false;
                }
                if(!_command_create(token, &((*c)->next))) {
                    delete_command(*c);
                    return false;
                }
                return true;
            case '&':
                (*c)->yield_type = YIELD_BACKGROUND;
                token = _get_next_token(token);
                if(!token) {
                    return true;
                }
                if(!_command_create(token, &((*c)->next))) {
                    delete_command(*c);
                    return false;
                }
                return true;
            default:
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
                a->index = ((*c)->argc)++;
                a->arg = token;
                a->next = NULL;
        }
    }
    return true;
}

bool new_command(char *line, Command **c) {
    char *name;
    name = strtok(line, _DLM);
    if(!name) {
        return false;
    }
    return _command_create(name, c);
}

void delete_command(Command *c) {
    Command *d;
    Argument *a, *b;
    FDTable *f, *g;

    free(c->name);
    while(c) {
        a = c->argv;
        while(a) {
            b = a->next;
            free(a);
            a = b;
        }
        f = c->fd_table;
        while(f) {
            if(fileno(f->from) > fileno(stderr)) {
                fclose(f->from);
            }
            if(fileno(f->to) > fileno(stderr)) {
                fclose(f->to);
            }
            g = f->next;
            free(f);
            f = g;
        }
        d = c->next;
        free(c);
        c = d;
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
