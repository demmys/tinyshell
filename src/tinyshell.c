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
    Command *c;
    char *line;
    pid_t pid;
    int status;

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
        pid = fork();
        if(pid < 0) {
            perror(argv[0]);
            return EXIT_FAILURE;
        } else if(pid == 0) {
            command_execute(c);
            perror(c->name);
            delete_command(c);
            return EXIT_FAILURE;
        }
        if(waitpid(pid, &status, 0) < 0) {
            perror(argv[0]);
            return EXIT_FAILURE;
        }
        delete_command(c);
    }
    return EXIT_SUCCESS;
}
