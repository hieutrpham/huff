#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

String *string_init(uint32_t size)
{
	String *str = (String*)malloc(sizeof(*str));
	if (!str)
		ERR(MALLOC_ERR);
	str->items = (char*)calloc(size + 1, sizeof(*str->items));
	if (!str->items)
		ERR(MALLOC_ERR);
	str->len = 0;
	str->cap = size;
	return str;
}

void string_destroy(String *str)
{
	if (str)
	{
		if (str->items)
		{
			free(str->items);
			str->items = NULL;
		}
		free(str);
		str = NULL;
	}
}

void string_append(String *str, const char *ext)
{
	size_t length = strlen(ext);
	void *new_buffer = NULL;

	if (str->len + length >= str->cap)
	{
		while (str->len + length >= str->cap)
			str->cap *= 2;
		new_buffer = realloc(str->items, str->cap);
		memset(new_buffer, '\0', str->cap);
		str->items = (char*)new_buffer;
		memcpy(&str->items[str->len], ext, length);
		str->len += length;
		return;
	}

	memcpy(&str->items[str->len], ext, length);
	str->len += length;
}
