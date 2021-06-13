#ifndef HASHING_H
#define HASHING_H

/* Type map_t is pointer to the struct map.
 * This way, struct map can not be instantiated outside of the module
 * and inner structure cannot be accessed by other means than the interface in this file.
 * */
typedef struct map *map_t;

// these functions need to be defined for each particular type of item
typedef uint (*hash_fn_t)(const void *item);
typedef  int  (*cmp_fn_t)(const void *item1, const void *item2);

// Read it the way of the compiler, you must.
// https://stackoverflow.com/questions/398395/why-is-the-asterisk-before-the-variable-name-rather-than-after-the-type

map_t map_create(uint max_size, hash_fn_t hash, cmp_fn_t cmp);
void  map_destroy(map_t *map);
void  map_insert(map_t *map, void *item);
void *map_search(map_t map, void *item);
void  map_delete(map_t map, void *item);
uint  map_get_size(map_t map);
uint  map_get_max_size(map_t map);
void  map_iterate(map_t map, uint *from, void **item);
void  map_resize(map_t *map, uint new_size);

#endif //HASHING_H
