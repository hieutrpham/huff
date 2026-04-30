#include "main.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
	c_freq value;
	struct Node *next;
} Node;

Node *create_new_node(c_freq v) {
	Node *new_node = malloc(sizeof(*new_node));
	if (!new_node) {
		fprintf(stderr, "ERR: malloc\n");
		exit(MALLOC_ERR);
	}
	new_node->value = v;
	new_node->next = NULL;
	return new_node;
}

// build the linked list given the frequency table
Node *build_list(hist_arr freq_table) {
// TODO:
}

int main() {
	Node *n = create_new_node((c_freq){.c = 'h', .freq = 2});
	printf("char: %c, freq: %zu\n", n->value.c, n->value.freq);
	free(n);
}
