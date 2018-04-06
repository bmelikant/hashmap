#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/hashmap.h"

extern long get_index(hashmap_t *map, char *key);

int main(int argc, char *argv[]) {

	// create a new, blank hashmap
	hashmap_t *myHashMap = create_hashmap(100, sizeof(int));

	int test = 1019823;
	add_key(myHashMap, "test key", (void*) &test);
	add_key(myHashMap, "kiki key", (void*) &test);

	// try to get the key back
	int *test2 = get_key(myHashMap, "test key");
	printf("The value of the test key was %d\n", *test2);

	int *test3 = get_key(myHashMap, "kiki key");
	printf("The value of kiki key is %d\n", *test3);
	
	printf("The key test key %s\n", (test2) ? "exists" : "does not exist");
	printf("The key kiki key %s\n", (test3) ? "exists" : "does not exist");

	printf("Index of test key: %li\n", get_index(myHashMap, "test key"));
	printf("Index of kiki key: %li\n", get_index(myHashMap, "kiki key"));

	return 0;
}