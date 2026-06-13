#include "main.h"

int main()
{
	String *str = string_init(4);
	string_append(str, "hello");
	printf("%s\n", str->items);
	string_append(str, "world");
	printf("%s\n", str->items);
	printf("%d\n", str->len);
	printf("%d\n", str->cap);
	string_append(str, "himom");
	printf("%s\n", str->items);
	string_append(str, "testing the string application");
	printf("%s\n", str->items);
	string_destroy(str);
}
