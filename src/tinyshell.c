#include "buffer.h"
#include "command.h"

typedef int (*Builtin)(const Argument *);

bool get_line_buffer(Buffer **b) {
    char ch;

    if(!new_buffer(512, b)) {
        return false;
    }
    for(ch = getchar(); ch != '\n'; ch = getchar()) {
        buffer_put(*b, ch);
    }
    return true;
}

int execute_cd(const Argument *argv) {
    char buf[512];
    getcwd(buf, 512);
    if(argv && !argv->next) {
        if(chdir(argv->arg) < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int execute_echo(const Argument *argv) {
    while(argv) {
        if(puts(argv->arg) < 0) {
            return EXIT_FAILURE;
        }
        argv = argv->next;
    }
    return EXIT_SUCCESS;
}

Builtin find_builtin(Command *c) {
    switch(c->name[0]){
        case 'c':
            if(strcmp(c->name, "cd") == 0) {
                return execute_cd;
            }
            break;
        case 'e':
            if(strcmp(c->name, "echo") == 0) {
                return execute_echo;
            }
    }
    return NULL;
}

int main(int argc, char const* argv[]) {
    Buffer *b;
    Command *c, *d;
    char *line;
    pid_t pid;
    int status;
    Builtin builtin;
    int piped = 0;
    int builtin_res;
    int stdin_back = 0, stdout_back = 0;

    while(true) {
        printf("@ ");

        if(!get_line_buffer(&b)) {
            perror(argv[0]);
            return EXIT_FAILURE;
        }
        line = buffer_restore(b);
        if(!new_command(line, &c)) {
            delete_buffer(b);
            perror(argv[0]);
            return EXIT_FAILURE;
        }
        delete_buffer(b);

        for(d = c; d; d = d->next) {
            builtin = find_builtin(d);
            if(builtin) {
                if(d->input != 0) {
                    stdin_back = dup(0);
                    dup2(d->input, 0);
                    close(d->input);
                }
                if(d->output != 1) {
                    stdout_back = dup(1);
                    dup2(d->output, 1);
                    close(d->output);
                }
                builtin_res = builtin(d->argv);
                if(stdin_back) {
                    dup2(stdin_back, 0);
                    close(stdin_back);
                    stdin_back = 0;
                }
                if(stdout_back) {
                    dup2(stdout_back, 1);
                    close(stdout_back);
                    stdout_back = 0;
                }
                if(builtin_res != EXIT_SUCCESS) {
                    perror(d->name);
                }
            } else {
                if(d->pipe) {
                    piped = d->output;
                }
                // fork process
                pid = fork();
                if(pid < 0) {
                    perror(argv[0]);
                    delete_command(c);
                    return EXIT_FAILURE;
                }
                if(pid == 0) {
                    if(d->input != 0) {
                        dup2(d->input, 0);
                        close(d->input);
                        if(piped > 0) {
                            close(piped);
                        }
                    }
                    if(d->output != 1) {
                        dup2(d->output, 1);
                        close(d->output);
                        if(d->pipe) {
                            close(d->next->input);
                        }
                    }
                    command_execute(d);
                    perror(d->name);
                    return EXIT_FAILURE;
                }
                if(d->input != 0) {
                    close(d->input);
                }
                if(d->output != 1) {
                    close(d->output);
                }
                if(!d->background) {
                    if(waitpid(pid, &status, 0) < 0) {
                        perror(argv[0]);
                        delete_command(c);
                        return EXIT_FAILURE;
                    }
                }
                if(!d->pipe && piped > 0) {
                    piped = 0;
                }
            }
        }
        delete_command(c);
    }

    return EXIT_SUCCESS;
}
