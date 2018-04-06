#include "../include/hashmap.h"

#include <string.h>

#define HASHMAP_SIZE_MIN		100		// hashmaps will be at least 100 elements
#define HASHMAP_REALLOCATE_SZ	100		// hashmaps will reallocate by additional 100 elements
#define HASHMAP_COLLISION_MAX	10		// collections with more than 10 items will recompute the hash table

struct HASHMAP {
	unsigned long collection_sz;
	collection_t *collection;
	size_t item_sz;
};

struct COLLECTION_ITEM {
	unsigned char *key;
	void *value;
};

struct COLLECTION {
	collection_item_t **mapped_items;
	size_t item_count;
};

/* STATIC FUNCTION FORWARD DECLARATIONS */
static unsigned long generate_string_hash(char *, hashmap_t *);
static void recompute_hashtable(hashmap_t *);
static collection_item_t *populate_key(char *key, void *value);
static int find_mapped_item(collection_item_t **collection, char *key);

// hashmap_t *create_hashmap(): Create a new hashmap with the given parameters
// inputs: items: The item collection to store, colleciton_size: the number of elements in the collection, element_size: Size of each element
// returns: a new hashmap structure
hashmap_t *create_hashmap(size_t collection_size, size_t element_size) {
	// make sure we allocate at least the minimum number of items
	if (collection_size < HASHMAP_SIZE_MIN) {
		collection_size = HASHMAP_SIZE_MIN;
	}

	// allocate a new hashmap object
	hashmap_t *new_hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
	new_hashmap->item_sz = element_size;
	new_hashmap->collection_sz = collection_size;

	// allocate the item collection and initialize it to null
	size_t collection_bytes = sizeof(collection_t)*collection_size;
	new_hashmap->collection = (collection_t *) malloc(collection_bytes);
	memset(new_hashmap->collection,0,collection_bytes);

	return new_hashmap;
}

// add_key(): Add a key/value pair to the hashmap
// inputs: hashmap - the hashmap, key - the item key, item - value of key
void add_key(hashmap_t *hashmap, char *key, void *item) {
	// generate a hash value from the string using the current collection size
	unsigned long hashval = generate_string_hash(key, hashmap);
	collection_t *hash_entry = (collection_t *) hashmap->collection+hashval;

	if (!hash_entry) {
		
		hash_entry->item_count = 1;
		hash_entry->mapped_items = (collection_item_t **) malloc(sizeof(collection_item_t **));
		hash_entry->mapped_items[0] = populate_key(key, item);

	} else {

		// the item is allocated, check for collisions
		if (hash_entry->item_count > HASHMAP_COLLISION_MAX) {
			
			//recompute_hashtable(hashmap);						// too many collisions; reallocate the table
			//add_key(hashmap, key, item);						// and run this function again
		
		} else {

			hash_entry->item_count++;
			hash_entry->mapped_items = (collection_item_t **) realloc(hash_entry->mapped_items, sizeof(collection_item_t **)*hash_entry->item_count);
			hash_entry->mapped_items[hash_entry->item_count-1] = populate_key(key, item);
		}
	}
}

// delete_key(): Remove a key/value pair from the hashmap
// inputs: key - key name to delete
// returns: nothing
void delete_key(hashmap_t *hashmap, char *key) {

}

// get_key(): Get the value of a key stored in the map
// inputs: hashmap - map to check, key - key to search
// returns: NULL on no item exists, value otherwise
void *get_key(hashmap_t *hashmap, char *key) {
	// get the lookup value
	unsigned long hashval = generate_string_hash(key, hashmap);
	collection_t *hash_entry = hashmap->collection+hashval;

	if (!hash_entry->mapped_items) {
		return NULL;
	} else if (hash_entry->item_count == 1) {
		return hash_entry->mapped_items[0]->value;
	} else {
		// try to find the correct item
		int item_idx = find_mapped_item(hash_entry->mapped_items, key);
		if (item_idx >= 0) {
			return hash_entry->mapped_items[item_idx]->value;
		} else {
			return NULL;
		}
	}
}

#ifdef DEBUG_BUILD
long get_index(hashmap_t *hashmap, char *key) {
	unsigned long hashval = generate_string_hash(key, hashmap);
	collection_t *hash_entry = hashmap->collection+hashval;

	if (!hash_entry->mapped_items) {
		return 0;
	} else {
		return (long) hashval;
	}
}
#endif

/* STATIC FUNCTIONS */

// generate_string_hash(): Create a hash value from a given string
// inputs: str - string to hash, hashmap - the map object for which to generate the string
// returns: a hashed value
static unsigned long generate_string_hash(char *str, hashmap_t *hashmap) {

	unsigned long h = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		unsigned long highorder = (unsigned long) h & 0xf8000000;
		h <<= 5;
		h ^= (highorder >> 27);
		h ^= (unsigned long)(str[i]);
	}
	return (unsigned long) h % hashmap->collection_sz;
}

// populate_key(): Create a new collection_item_t from a given key/value pair
// inputs: key - string value of key, value - data to store
// returns: new key/value item
static collection_item_t *populate_key(char *key, void *value) {
	collection_item_t *new_key = (collection_item_t *) malloc(sizeof(collection_item_t));
	new_key->key = (char *) malloc(strlen(key));
	strncpy(new_key->key,key,strlen(key));
	new_key->value = value;
	return new_key;
}

// find_mapped_item(): Get the index of a mapped item if it exists in a key
// inputs: collection: collection items to search, key: key to find
// returns: mapped item index
static int find_mapped_item(collection_item_t **collection, char *key) {
	int index = 0;
	collection_item_t *item = collection[index];

	while (item) {
		if (!strncmp(key, item->key,strlen(key))) {
			return index;
		}
		item = (collection_item_t *) item+sizeof(collection_item_t *);
		index++;
	}

	return -1;
}

// recompute_hashtable(): reallocates a given hash table
