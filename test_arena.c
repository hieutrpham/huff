#define ARENA_CAP 1
#define BLOCK_CAP 4
#include "main.h"
#include "arena.c"

int main()
{
	struct Arena* a = arena_init();
	printf("arena cap: %d - %d\n", ARENA_CAP, a->cap);
	void *mem = arena_malloc(a, 3);
	printf("mem: %p\n", mem);
	void *mem1 = arena_malloc(a, 1);
	void *mem2 = arena_malloc(a, 1);
	printf("mem: %p\n", mem);
	// void *mem1 = arena_malloc(a, 4);
	void *mem3 = arena_malloc(a, 3);
	printf("mem3: %p\n", mem3);

	void *mem4 = arena_malloc(a, 3);
	printf("mem4: %p\n", mem4);

	void *mem5 = arena_malloc(a, 3);
	printf("mem5: %p\n", mem5);

	void *mem6 = arena_malloc(a, 3);
	printf("mem6: %p\n", mem6);
	printf("arena cap: %d - %d\n", ARENA_CAP, a->cap);
	arena_destroy(a);
}
