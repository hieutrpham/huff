#include "main.h"

// arena contains an array of pointers of blocks of memory
// when the first block is full, increase the arena cap
struct Arena *arena_init()
{
	struct Arena* a = (struct Arena*)malloc(sizeof(*a));
	if (!a)
		ERR(MALLOC_ERR);
	a->blocks = (struct Block**)malloc(ARENA_CAP * sizeof(*a->blocks));
	if (!a->blocks)
		ERR(MALLOC_ERR);

	a->active_block_index = 0;
	a->cap                = ARENA_CAP;

	for (uint i = 0; i < a->cap; ++i)
	{
		a->blocks[i] = (struct Block*)malloc(sizeof(*a->blocks[i]));
		if (!a->blocks[i])
			ERR(MALLOC_ERR);
		a->blocks[i]->len = 0;
	}
	return a;
}

void *arena_malloc(struct Arena* a, uint32_t size)
{
	if (size > BLOCK_CAP)
	{
		fprintf(stderr, "Size too big. Maximum size is: %d\n", BLOCK_CAP);
		exit(MALLOC_ERR);
	}

	struct Block *active_block = a->blocks[a->active_block_index];
	if (size + active_block->len > BLOCK_CAP)
	{
		if (a->active_block_index + 1 >= a->cap)
		{
			while (a->active_block_index + 1 >= a->cap)
				a->cap *= 2;
			struct Block **new_blocks = (struct Block**)realloc(a->blocks, a->cap * sizeof(*new_blocks));
			if (!new_blocks)
				ERR(MALLOC_ERR);

			for (uint i = a->active_block_index + 1; i < a->cap; ++i)
			{
				new_blocks[i] = (struct Block*)malloc(sizeof(*new_blocks[i]));
				memset(new_blocks[i]->data, 0, sizeof(new_blocks[i]->data)/sizeof(*new_blocks[i]->data));
				new_blocks[i]->len = 0;
				if (!new_blocks[i])
					ERR(MALLOC_ERR);
			}
			a->blocks = new_blocks;
		}
		a->active_block_index++;
	}
	
	active_block = a->blocks[a->active_block_index];
	void *mem    = &active_block->data[active_block->len];
	active_block->len += size;
	return mem;
}

void arena_destroy(struct Arena* a)
{
	if (a)
	{
		if (a->blocks)
		{
			for (uint i = 0; i < a->cap; ++i)
			{
				free(a->blocks[i]);
				a->blocks[i] = NULL;
			}
			free(a->blocks);
			a->blocks = NULL;
		}
		free(a);
		a = NULL;
	}
}
