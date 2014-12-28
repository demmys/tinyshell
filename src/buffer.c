#include "buffer.h"

bool new_buffer(int capacity, Buffer **b) {
    *b = (Buffer *)malloc(sizeof(Buffer));
    if(NULL == *b) {
        return false;
    }
    (*b)->capacity = capacity;
    (*b)->size = 0;
    (*b)->contents = (char *)malloc(sizeof(char) * capacity);
    (*b)->next = NULL;
    return true;
}

void delete_buffer(Buffer *b) {
    Buffer *c;
    while(b) {
        c = b->next;
        free(b->contents);
        free(b);
        b = c;
    }
}

bool buffer_put(Buffer *b, char c) {
    if(!b) {
        return false;
    }
    while(b->next) {
        b = b->next;
    }
    if(b->size == b->capacity) {
        if(!new_buffer(b->capacity, &(b->next))) {
            return false;
        }
        b = b->next;
    }
    (b->contents)[(b->size)++] = c;
    return true;
}

bool buffer_store(Buffer *b, const char *str) {
    while(*str) {
        if(!buffer_put(b, *str)) {
            return false;
        }
        ++str;
    }
    return true;
}

char *buffer_restore(const Buffer *b) {
    int total_size;
    const Buffer *c;
    char *r, *s;

    total_size = 0;
    c = b;
    while(c) {
        total_size += c->size;
        c = c->next;
    }
    if(total_size == 0) {
        return NULL;
    }

    r = (char *)malloc(sizeof(char) * total_size + 1);
    s = r;
    while(b) {
        strncpy(s, b->contents, b->size);
        s = s + b->size;
        b = b->next;
    }
    *s = '\0';
    return r;
}
