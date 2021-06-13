#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/hashing.h"

uint hash_fn(const void *);
int   cmp_fn(const void *, const void *);

START_TEST(empty_test) {} END_TEST

START_TEST(creates_empty_map) {
    uint size = 5;
    map_t map = map_create(size, &hash_fn, &cmp_fn);    // https://stackoverflow.com/a/16917159
    ck_assert_int_eq(map_get_size(map), 0);
    map_destroy(&map);
    ck_assert_ptr_eq(map, NULL);
} END_TEST

START_TEST(creates_map_of_the_correct_size) {
    uint sizes[] = {5, 8, 11};
    for (uint i=0; i<3; i++) {
        map_t map = map_create(sizes[i], &hash_fn, &cmp_fn);
        ck_assert_int_eq(map_get_max_size(map), sizes[i]);
        map_destroy(&map);
        ck_assert_ptr_eq(map, NULL);
    }
} END_TEST

START_TEST(can_insert_one_item) {
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    uint *item = malloc(sizeof *item);
    (*item) = 3;
    map_insert(&map, item);
    ck_assert_int_eq(map_get_size(map), 1);
} END_TEST

START_TEST(can_insert_more_items) {
    uint items[] = {3, 5, 7, 11};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<4; i++) { map_insert(&map, &items[i]); }
    ck_assert_int_eq(map_get_size(map), 4);
} END_TEST

START_TEST(resizes_map) {
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    map_resize(&map, 23);
    ck_assert_int_eq(map_get_max_size(map), 23);
} END_TEST

START_TEST(inserted_items_can_be_found) {
    uint items[] = {7, 11, 18, 57, 68};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<5; i++) { map_insert(&map, &items[i]); }
    for (uint i=0; i<5; i++) {
        uint *item = map_search(map, &items[i]);
        ck_assert_int_eq(*item, items[i]);
    }
} END_TEST

START_TEST(inserting_more_items_than_max_size_resizes_map) {
    uint items[] = {7, 54, 84, 42, 13, 56, 45};
    map_t map = map_create(4, &hash_fn, &cmp_fn);
    for (uint i=0; i<7; i++) { map_insert(&map, &items[i]); }
    ck_assert_uint_ge(map_get_max_size(map), 7);
    ck_assert_uint_eq(map_get_size(map), 7);
} END_TEST

START_TEST(deleted_item_can_not_be_found) {
    uint items[] = {49};
    map_t map = map_create(11, &hash_fn, &cmp_fn);
    map_insert(&map, &items[0]);
    uint *item = map_search(map, &items[0]);
    ck_assert_uint_eq(*item, items[0]);
    map_delete(map, &items[0]);
    item = map_search(map, &items[0]);
    ck_assert_ptr_eq(item, NULL);
} END_TEST

START_TEST(items_with_the_same_hash_deleted) {
    uint items[] = {3, 13, 23};
    map_t map = map_create(5, &hash_fn, &cmp_fn);
    for (uint i=0; i<3; i++) map_insert(&map, &items[i]);
    map_delete(map, &items[1]);
    map_delete(map, &items[2]);
    uint *item = map_search(map, &items[0]);
    ck_assert_uint_eq(*item, items[0]);
    item = map_search(map, &items[1]);
    ck_assert_ptr_eq(item, NULL);
    item = map_search(map, &items[2]);
    ck_assert_ptr_eq(item, NULL);
} END_TEST

START_TEST(inserting_and_deleting_should_not_change_the_size) {
    uint items[] = {4, 9, 65, 12};
    map_t map = map_create(20, &hash_fn, &cmp_fn);
    map_insert(&map, &items[1]);
    map_delete(map, &items[1]);
    ck_assert_uint_eq(map_get_size(map), 0);
} END_TEST

START_TEST(deleting_the_item_with_the_same_hash_should_be_unsuccessfull) {
    uint items[] = {5, 15};
    map_t map = map_create(10, &hash_fn, &cmp_fn);
    map_insert(&map, &items[1]);
    map_delete(map, &items[0]);
    ck_assert_uint_eq(map_get_size(map), 1);
} END_TEST

START_TEST(resizing_to_smaller_size_than_number_of_items) {
    uint items[] = {5, 48, 65, 21, 25, 54, 56, 59, 42, 15};
    map_t map = map_create(10, &hash_fn, &cmp_fn);
    for (uint i=0; i<10; i++) map_insert(&map, &items[i]);
    ck_assert_uint_eq(map_get_size(map), 10);
    map_resize(&map, 1);
    ck_assert_uint_eq(map_get_max_size(map), 16);
    ck_assert_uint_eq(map_get_size(map), 10);
} END_TEST

int main(void) {
    int failed;
    Suite *suite = suite_create("suite");
    TCase *tcase = tcase_create("tcase");
    suite_add_tcase(suite, tcase);

    tcase_add_test(tcase, empty_test);
    tcase_add_test(tcase, creates_empty_map);
    tcase_add_test(tcase, creates_map_of_the_correct_size);
    tcase_add_test(tcase, can_insert_one_item);
    tcase_add_test(tcase, can_insert_more_items);
    tcase_add_test(tcase, resizes_map);
    tcase_add_test(tcase, inserted_items_can_be_found);
    tcase_add_test(tcase, inserting_more_items_than_max_size_resizes_map);
    tcase_add_test(tcase, deleted_item_can_not_be_found);
    tcase_add_test(tcase, items_with_the_same_hash_deleted);
    tcase_add_test(tcase, inserting_and_deleting_should_not_change_the_size);
    tcase_add_test(tcase, deleting_the_item_with_the_same_hash_should_be_unsuccessfull);
    tcase_add_test(tcase, resizing_to_smaller_size_than_number_of_items);

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    // srunner_run_all(runner, CK_VERBOSE);
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

