#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/hashing.h"
#include "../src/hashing.c"

uint hash_fn(const void *);
int   cmp_fn(const void *, const void *);

START_TEST(empty_test) {} END_TEST

START_TEST(creates_empty_map) {
    uint size = 5;
    map_t *map = map_create(size, &hash_fn, &cmp_fn);    // https://stackoverflow.com/a/16917159
    ck_assert_int_eq(map_get_size(map), 0);
    map_destroy(map);
    map = NULL;
} END_TEST

START_TEST(creates_map_of_the_correct_size) {
    uint sizes[] = {5, 8, 11};
    for (uint i=0; i<3; i++) {
        map_t *map = map_create(sizes[i], &hash_fn, &cmp_fn);
        ck_assert_int_eq(map_get_max_size(map), sizes[i]);
        map_destroy(map);
        map = NULL;
    }
} END_TEST

//START_TEST(destroyed_map_is_deallocated) {
//    // allocate 32GB in chunks of 1GB
//    // if you run out of memory, the deallocation does not work
//    uint size = 1024 * 1024 * 1024 / sizeof (void *);
//    for (uint i=0; i<32; i++) {
//        map_t *map = map_create(size, &hash_fn, &cmp_fn);
//        map_destroy(map);
//    }
//    // find some other way of testing?
//} END_TEST

START_TEST(can_insert_one_item) {
    map_t *map = map_create(5, &hash_fn, &cmp_fn);
    uint *item = malloc(sizeof *item);
    (*item) = 3;
    map_insert(map, item);
    ck_assert_int_eq(map_get_size(map), 1);
} END_TEST

START_TEST(can_insert_more_items) {
    uint items[] = {3, 5, 7, 11};
    map_t *map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(map, &items[i]); }
    ck_assert_int_eq(map_get_size(map), 4);
} END_TEST

START_TEST(resizes_map) {
    map_t *map = map_create(5, &hash_fn, &cmp_fn);
    map_resize(map, 23);
    ck_assert_int_eq(map_get_max_size(map), 23);
} END_TEST

START_TEST(inserted_items_can_be_found) {
    uint items[] = {7, 11, 18, 57, 68};
    map_t *map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<5; i++) { map_insert(map, &items[i]); }
    for (uint i=0; i<5; i++) {
        uint *item = map_search(map, &items[i]);
        ck_assert_int_eq(*item, items[i]);
    }
} END_TEST

int main(void) {
    int failed;
    Suite *suite = suite_create("suite");
    TCase *tcase = tcase_create("tcase");
    suite_add_tcase(suite, tcase);

    tcase_add_test(tcase, empty_test);
    tcase_add_test(tcase, creates_empty_map);
    tcase_add_test(tcase, creates_map_of_the_correct_size);
    // tcase_add_test(tcase, destroyed_map_is_deallocated);
    tcase_add_test(tcase, can_insert_one_item);
    tcase_add_test(tcase, can_insert_more_items);
    tcase_add_test(tcase, resizes_map);
    tcase_add_test(tcase, inserted_items_can_be_found);

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
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

