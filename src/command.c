#include "command.h"

static const char _DLM[3] = " \t";

static bool _malloc_command(char *name, Command **c) {
    Command *d;
    *c = (Command *)malloc(sizeof(Command));
    if(!*c) {
        return false;
    }
    d = *c;
    d->name = name;
    d->argc = 0;
    d->argv = NULL;
    d->input = 0;
    d->output = 1;
    d->background = false;
    d->next = NULL;
    return true;
}

static char *_get_next_token(char *token) {
    if(token[1] == '\0') {
        return strtok(NULL, _DLM);
    } else {
        return token + 1;
    }
}

static bool _create_command(char *name, Command **c) {
    Argument *a;
    char *token;
    int fd;
    int pipefd[2];

    if(!_malloc_command(name, c)) {
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
                fd = open(token, O_RDONLY);
                if(fd < 0) {
                    delete_command(*c);
                    return false;
                }
                (*c)->input = fd;
                break;
            case '>':
                token = _get_next_token(token);
                if(!token) {
                    delete_command(*c);
                    return false;
                }
                fd = open(token, O_WRONLY | O_CREAT, 0640);
                if(fd < 0) {
                    delete_command(*c);
                    return false;
                }
                (*c)->output = fd;
                break;
            case '|':
                if(pipe(pipefd) < 0) {
                    delete_command(*c);
                    return false;
                }
                (*c)->output = pipefd[1];
                token = _get_next_token(token);
                if(!token) {
                    delete_command(*c);
                    return false;
                }
                if(!_create_command(token, &((*c)->next))) {
                    delete_command(*c);
                    return false;
                }
                (*c)->next->input = pipefd[0];
                (*c)->pipe = true;
                return true;
            case '&':
                (*c)->background = true;
                token = _get_next_token(token);
                if(!token) {
                    return true;
                }
                if(!_create_command(token, &((*c)->next))) {
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
    return _create_command(name, c);
}

void delete_command(Command *c) {
    Command *d;
    Argument *a, *b;

    free(c->name);
    while(c) {
        a = c->argv;
        while(a) {
            b = a->next;
            free(a);
            a = b;
        }
        d = c->next;
        free(c);
        c = d;
    }
}

void command_execute(Command *c) {
    Argument *a;
    char **argv;
    int i;

    argv = (char **)malloc(sizeof(char *) * (c->argc + 2));
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
