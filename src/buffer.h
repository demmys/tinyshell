#ifndef SHELL_BUFFER_H_INCLUDED
#define SHELL_BUFFER_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Buffer_tag {
    int capacity;
    int size;
    char *contents;
    struct Buffer_tag *next;
} Buffer;

bool new_buffer(int capacity, Buffer **b);
void delete_buffer(Buffer *b);
bool buffer_put(Buffer *b, char c);
bool buffer_store(Buffer *b, const char *str);
char *buffer_restore(const Buffer *b);

#endif
