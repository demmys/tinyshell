#include "command.h"

int main(int argc, char const* argv[]) {
    bool res;
    Command *c;
    pid_t pid;
    int status;

    while(true) {
        printf("@ ");
        res = new_command(&c);
        if(!res) {
            perror(argv[0]);
            return EXIT_FAILURE;
        }
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
