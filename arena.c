#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct arena *arena_init(uint32_t capacity)
{
	struct arena* a = malloc(sizeof(*a));
	if (!a)
		ERR(MALLOC_ERR);
	a->items = calloc(capacity + 1, sizeof(*a->items));
	if (!a->items)
		ERR(MALLOC_ERR);
	a->len = 0;
	a->cap = capacity;
	return a;
}

void arena_destroy(struct arena* a)
{
	if (a)
	{
		if (a->items)
		{
			free(a->items);
			a->items = NULL;
		}
		free(a);
		a = NULL;
	}
}

void *arena_malloc(struct arena* a, uint32_t size)
{
	assert(size + a->len < a->cap);

	// TODO: reallocation
	if (size + a->len >= a->cap)
	{
		return NULL;
	}

	void *mem = &a->items[a->len];
	a->len += size;
	return mem;
}
