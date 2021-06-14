#include <stdio.h>
#include <stdlib.h>
#include "safe_alloc.h"

void *safe_malloc(uint size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: malloc() unsuccessful.");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void safe_free(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}