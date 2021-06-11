#include <stdio.h>
#include <stdlib.h>
#include "hashing.h"

uint hash(const void *item) {
    uint *n = (uint *)item;
    return *n;
}

void print_item(void *item) {
    if (item == NULL) {
        printf(" NULL ");
    } else {
        int *ptr = (int *)item;
        printf(" %i ", *ptr);
    }
}

void set_print(map_t *map) {
    uint size = map_get_size(map);
    printf("%i: {", size);

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

int cmp(const void *item1,const void *item2) {
    const int *i1 = item1;
    const int *i2 = item2;
    if (*i1 < *i2) return -1;
    else if (*i1 > *i2) return 1;
    else return 0;
}

int main() {
    uint i = 0;
    map_t *map = map_create(11, hash, cmp);

    uint *a = malloc(sizeof *a);
    (*a) = 10;
    map_insert(map, a);
    a = malloc(sizeof *a);
    (*a) = 5;
    map_insert(map, a);
    set_print(map);

//    map_resize(map, 20);
//    set_print(map);
//
//    a = map_search(map, a);
//    printf("%i\n", *a);
//
//    map_destroy(map);

    return 0;
}
