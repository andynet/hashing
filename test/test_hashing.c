#include <check.h>
#include <stdlib.h>
#include "../src/hashing.h"

uint hash_fn(const void *);
int   cmp_fn(const void *, const void *);

START_TEST(empty_test){} END_TEST

START_TEST(creates_empty_map){
    uint size = 5;
    map_t *map = map_create(size, &hash_fn, &cmp_fn);    // https://stackoverflow.com/a/16917159
    ck_assert_int_eq(map_get_size(map), 0);
} END_TEST

int main(void) {
    int failed;
    Suite *suite = suite_create("suite");
    TCase *tcase = tcase_create("tcase");
    suite_add_tcase(suite, tcase);

    tcase_add_test(tcase, empty_test);
    tcase_add_test(tcase, creates_empty_map);

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

