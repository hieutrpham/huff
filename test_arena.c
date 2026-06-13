#include "main.h"
#include <stdint.h>
#include <sys/types.h>

int main()
{
	struct arena* a = arena_init(4);
	printf("items pointer: %p\n", a->items);
	void* mem = &a->items[arena_malloc(a, 1)];
	*(int*)mem = 69;
	printf("mem before realloc: %d\n", *(int*)mem);
	printf("arena len: %d\n", a->len);
	void *mem2 = h_malloc(a, 90);
	printf("items pointer: %p\n", a->items);
	printf("mem after realloc: %d\n", *mem);

	// after reallocation, mem doesn't have the same value
	printf("arena cap: %d\n", a->cap);
	// printf("mem2: %p\n", mem2);
	// printf("arena len: %d\n", a->len);
	// printf("items pointer: %p\n", a->items);
	// printf("arena cap: %d\n", a->cap);
	arena_destroy(a);
}
