#include <stdio.h>
#include <stdlib.h>
#include "src/hashing.h"

uint hash_fn(const void *item) {
    uint *n = (uint *)item;
    return *n;
}

void print_item(void *item) {
    if (item == NULL) {
        printf("NULL, ");
    } else {
        int *ptr = (int *)item;
        printf("%i, ", *ptr);
    }
    fflush(stdout); // The stdout is flushed only on newline by default.
}

void set_print(map_t map) {
    uint size = map_get_size(map);
    uint max_size = map_get_max_size(map);
    printf("%i/%i: {", size, max_size);

    uint i=0;
    void *a=NULL;
    map_iterate(map, &i, &a);
    while (a != NULL) {
        printf("%i: ", i);
        print_item(a);
        i++;
        map_iterate(map, &i, &a);
    }
    printf("}\n");
}

int cmp_fn(const void *item1,const void *item2) {
    const int *i1 = item1;
    const int *i2 = item2;
    if (*i1 < *i2) return -1;
    else if (*i1 > *i2) return 1;
    else return 0;
}

int main() {

    map_t map = map_create(4, &hash_fn, &cmp_fn);
    uint *absent = malloc(sizeof *absent);
    *absent = 10;
    uint *item = map_search(map, absent);
    map_destroy(&map);

    return 0;
}
