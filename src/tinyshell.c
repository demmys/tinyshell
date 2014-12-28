#include "command.h"

int main(int argc, char const* argv[]) {
    bool res;
    Command *c;

    while(true) {
        printf("@ ");
        res = new_command(&c);
        if(!res) {
            return EXIT_FAILURE;
        }
        command_execute(c);
        delete_command(c);
    }
    return EXIT_SUCCESS;
}
