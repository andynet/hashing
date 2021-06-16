#include <criterion/criterion.h>
#include "../src/hashing.h"

uint hash_fn(const void *);
int   cmp_fn(const void *, const void *);

Test(tmp, test1) {
    uint size = 5;
    map_t map = map_create(size, &hash_fn, &cmp_fn);    // https://stackoverflow.com/a/16917159
    cr_assert(map_get_size(map) == 0);
    map_destroy(&map);
    cr_assert(map == NULL);
}

uint hash_fn(const void *item) {
    uint *n = (uint *)item;
    return *n;
}

int cmp_fn(const void *item1, const void *item2) {
    const int *i1 = item1;
    const int *i2 = item2;
    if (*i1 < *i2) return -1;
    else if (*i1 > *i2) return 1;
    else return 0;
}
