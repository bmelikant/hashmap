#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stdlib.h>
#include <stddef.h>

#ifndef _cplusplus
#include <stdbool.h>
#endif

// associated structures
typedef struct COLLECTION_ITEM collection_item_t;
typedef struct COLLECTION collection_t;
typedef struct HASHMAP hashmap_t;

// API functions
hashmap_t *create_hashmap(size_t collection_size, size_t element_sz);
void add_key(hashmap_t *hashmap, char *key, void *value);
void delete_key(hashmap_t *hashmap, char *key);
void *get_key(hashmap_t *hashmap, char *key);
bool key_exists(hashmap_t *hashmap, char *key);

#endif