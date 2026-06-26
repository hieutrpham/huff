#define ARENA_CAP 1
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
	struct Arena* a = arena_init();
	printf("%d\n", a->cap);
	void *mem = arena_malloc(a, 3);
	printf("mem: %p\n", mem);
	void *mem1 = arena_malloc(a, 1);
	void *mem2 = arena_malloc(a, 1);
	printf("mem: %p\n", mem);
	// void *mem1 = arena_malloc(a, 4);
	printf("mem1: %p\n", mem1);
	arena_malloc(a, 3);
	arena_malloc(a, 3);
	arena_malloc(a, 3);
	arena_malloc(a, 3);
	arena_destroy(a);
}
