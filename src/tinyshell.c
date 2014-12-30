#include "buffer.h"
#include "command.h"

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

int main(int argc, char const* argv[]) {
    Buffer *b;
    Command *c, *d;
    char *line;
    pid_t pid;
    int status;
    int piped = 0;

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
                if(d->output != 1) {
                    dup2(d->output, 1);
                    if(d->pipe) {
                        close(d->next->input);
                    }
                }
                if(d->input != 0) {
                    dup2(d->input, 0);
                    if(piped > 0) {
                        close(piped);
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
        delete_command(c);
    }

    return EXIT_SUCCESS;
}
