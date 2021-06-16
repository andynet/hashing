#include <stdio.h>
#include <stdlib.h>
#include "hashing.h"

/* This is an address which is guaranteed to be NOT used for the objects item.
 * Some discussion about this is here:
 * https://stackoverflow.com/questions/3304795/can-a-pointer-address-ever-be-negative
 * and
 * https://www.youtube.com/watch?v=2Ti5yvumFTU&t=1054s */
char g_tombstone = '\0';
#define DELETED ((void *)&g_tombstone)



struct map {
    uint max_size;
    uint size;
    hash_fn_t hash_fn;
    cmp_fn_t cmp_fn;
    void **items;
};

map_t map_create(uint max_size, hash_fn_t hash_fn, cmp_fn_t cmp_fn) {
    map_t map = safe_malloc(sizeof *map);
    map->max_size = max_size;
    map->size = 0;
    map->hash_fn = hash_fn;
    map->cmp_fn = cmp_fn;
    map->items = safe_malloc(sizeof *(map->items) * map->max_size);
    for (uint i=0; i<map->max_size; i++) { map->items[i] = NULL; }
    return map;
}

/* Deallocates and sets map to NULL.
 * This does not deallocate the items themselves, only the map.
 * To deallocate the items, the map can be iterated with map_iterate
 * and items deallocated one by one. */
void   map_destroy(map_t *map) {
    safe_free((void **) &(*map)->items);
    safe_free((void **) map);
}

/* Uses linear probing, possible resizing. */
void   map_insert(map_t *map, void *item) {
    if ((*map)->size == (*map)->max_size) {
        map_resize(map, (*map)->max_size * 2);
    }

    uint idx = (*map)->hash_fn(item) % (*map)->max_size;
    while ((*map)->items[idx] != NULL && (*map)->items[idx] != DELETED) {
        idx = (idx + 1) % (*map)->max_size;
    }
    (*map)->items[idx] = item;
    (*map)->size++;
}

/* Returns NULL if item is not found. */
void  *map_search(map_t map, void *item) {
    uint idx = map->hash_fn(item) % map->max_size;
    void *current = map->items[idx];
    uint i = 0;
    while ((current == DELETED || map->cmp_fn(current, item) != 0) && (i<map->max_size)) {
        idx = (idx + 1) % map->max_size;
        current = map->items[idx];
        if (current == NULL) return NULL;
        i++;
    }

    if (map->cmp_fn(current, item) == 0) return map->items[idx];
    else return NULL;
}

/* using tombstones */
void   map_delete(map_t map, void *item) {
    uint idx = map->hash_fn(item) % map->max_size;
    void *current = map->items[idx];
    if (current == NULL) { return; }
    uint i = 0;
    while ((current == DELETED || map->cmp_fn(current, item) != 0) && (i<map->max_size)) {
        idx = (idx + 1) % map->max_size;
        current = map->items[idx];
        if (current == NULL) { return; }
        i++;
    }

    if (map->cmp_fn(current, item) == 0) {
        map->items[idx] = DELETED;
        map->size--;
    } else {
        return;
    }
}

uint   map_get_size(map_t map) { return map->size; }
uint   map_get_max_size(map_t map) { return map->max_size; }

void   map_iterate(map_t map, uint *from, void **item) {
    void *current = NULL;
    while (current == NULL || current == DELETED) {
        if ((*from) == map->max_size) {
            (*item) = NULL;
            return;
        }
        current = map->items[*from];
        (*from)++;
    }
    (*item) = current;
    (*from)--;
}

void   map_resize(map_t *map, uint new_size) {
    map_t old_map = *map;
    while (new_size < old_map->size) { new_size *= 2; }
    map_t new_map = map_create(new_size, old_map->hash_fn, old_map->cmp_fn);

    uint idx = 0;
    void *item = NULL;
    map_iterate(old_map, &idx, &item);
    while (item != NULL) {
        map_insert(&new_map, item);
        idx++;
        map_iterate(old_map, &idx, &item);
    }

    map_destroy(map);
    *map = new_map;
}
