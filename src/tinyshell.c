#include <stdio.h>
#include <stdlib.h>
#include "command.h"

int run_command(Command *c) {
    while(c) {
        printf("|%s|\n", c->contents);
        c = c->next;
    }
    return 0;
}

int main(int argc, char const* argv[]) {
    Command *c;

    while(true) {
        printf("@ ");
        c = new_command();
        run_command(c);
        delete_command(c);
    }
    return EXIT_SUCCESS;
}
