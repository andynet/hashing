#include <criterion/criterion.h>
#include "../src/hashing.h"

uint hash_fn(const void *);
int   cmp_fn(const void *, const void *);

Test(core, empty_test) {}

Test(core, creates_empty_map) {
    uint size = 5;
    map_t map = map_create(size, &hash_fn, &cmp_fn);    // https://stackoverflow.com/a/16917159
    cr_assert(map_get_size(map) == 0);
    map_destroy(&map);
    cr_assert(map == NULL);
}

Test(core, creates_map_of_the_correct_size) {
    uint sizes[] = {5, 8, 11};
    for (uint i=0; i<3; i++) {
        map_t map = map_create(sizes[i], &hash_fn, &cmp_fn);
        cr_assert(map_get_max_size(map) == sizes[i]);
        map_destroy(&map);
        cr_assert(map == NULL);
    }
}

Test(core, can_insert_one_item) {
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    uint *item = malloc(sizeof *item);
    (*item) = 3;
    map_insert(&map, item);
    cr_assert(map_get_size(map) == 1);
    map_destroy(&map);
}

Test(core, can_insert_more_items) {
    uint items[] = {3, 5, 7, 11};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(&map, &items[i]); }
    cr_assert(map_get_size(map) == 4);
    map_destroy(&map);
}

Test(core, resizes_map) {
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    map_resize(&map, 23);
    cr_assert(map_get_max_size(map) == 23);
    map_destroy(&map);
}

Test(core, inserted_items_can_be_found) {
    uint items[] = {7, 11, 18, 57, 68};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<5; i++) { map_insert(&map, &items[i]); }
    for (uint i=0; i<5; i++) {
        uint *item = map_search(map, &items[i]);
        cr_assert(*item == items[i]);
    }
    map_destroy(&map);
}

Test(core, inserting_more_items_than_max_size_resizes_map) {
    uint items[] = {7, 54, 84, 42, 13, 56, 45};
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    for (uint i=0; i<7; i++) { map_insert(&map, &items[i]); }
    cr_assert(map_get_max_size(map) >= 7);
    cr_assert(map_get_size(map) == 7);
    map_destroy(&map);
}

Test(core, deleted_item_can_not_be_found) {
    uint items[] = {49};
    map_t map = map_create(11, &hash_fn, &cmp_fn);
    map_insert(&map, &items[0]);
    uint *item = map_search(map, &items[0]);
    cr_assert(*item == items[0]);
    map_delete(map, &items[0]);
    item = map_search(map, &items[0]);
    cr_assert(item == NULL);
    map_destroy(&map);
}

Test(core, items_with_the_same_hash_deleted) {
    uint items[] = {3, 13, 23};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<3; i++) map_insert(&map, &items[i]);
    map_delete(map, &items[1]);
    map_delete(map, &items[2]);
    uint *item = map_search(map, &items[0]);
    cr_assert(*item == items[0]);
    item = map_search(map, &items[1]);
    cr_assert(item == NULL);
    item = map_search(map, &items[2]);
    cr_assert(item == NULL);
    map_destroy(&map);
}

Test(core, inserting_and_deleting_should_not_change_the_size) {
    uint items[] = {4, 9, 65, 12};
    map_t map = map_create(20, &hash_fn, &cmp_fn);
    map_insert(&map, &items[1]);
    map_delete(map, &items[1]);
    cr_assert(map_get_size(map) == 0);
    map_destroy(&map);
}

Test(core, deleting_the_item_with_the_same_hash_should_be_unsuccessfull) {
    uint items[] = {5, 15};
    map_t map = map_create(10, &hash_fn, &cmp_fn);
    map_insert(&map, &items[1]);
    map_delete(map, &items[0]);
    cr_assert(map_get_size(map) == 1);
    map_destroy(&map);
}

Test(core, resizing_to_smaller_size_than_number_of_items) {
    uint items[] = {5, 48, 65, 21, 25, 54, 56, 59, 42, 15};
    map_t map = map_create(10, &hash_fn, &cmp_fn);
    for (uint i=0; i<10; i++) map_insert(&map, &items[i]);
    cr_assert(map_get_size(map) == 10);
    map_resize(&map, 1);
    cr_assert(map_get_max_size(map) == 16);
    cr_assert(map_get_size(map) == 10);
    map_destroy(&map);
}

Test(core, finding_nonexistent_item_from_full_map_should_return_NULL) {
    uint items[] = {24, 48, 12, 65};
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(&map, &items[i]); }
    uint *absent = malloc(sizeof *absent);
    *absent = 10;
    uint *item = map_search(map, absent);
    cr_assert(item == NULL);
    map_destroy(&map);
}

Test(core, searching_empty_map_should_return_NULL) {
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    uint *absent = malloc(sizeof *absent);
    *absent = 10;
    uint *item = map_search(map, absent);
    cr_assert(item == NULL);
    map_destroy(&map);
}

Test(core, deleting_nonexistent_item_from_full_map_should_end) {
    uint items[] = {24, 48, 12, 65};
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(&map, &items[i]); }
    uint *absent = malloc(sizeof *absent);
    *absent = 10;
    map_delete(map, absent);
    map_destroy(&map);
}

Test(core, iterating_full_map_should_end) {
    uint items[] = {24, 48, 12, 65};
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(&map, &items[i]); }

    uint idx = 0;
    void *item = NULL;
    map_iterate(map, &idx, &item);
    while (item != NULL) {
        idx++;
        map_iterate(map, &idx, &item);
    }
    map_destroy(&map);
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
