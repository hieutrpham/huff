#include "main.h"

Node *create_new_node(c_freq v) {
	Node *new_node = malloc(sizeof(*new_node));
	if (!new_node) {
		fprintf(stderr, "ERR: malloc\n");
		exit(MALLOC_ERR);
	}
	new_node->value = v;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// build the linked list given the frequency table
/*
 *  ------------       ------------ 
 * |value: |c   |     |value: |c   |
 * |       |freq|  -> |       |freq|
 * |Node* next  |     |Node* next  |
 * |Node* prev  |     |Node* prev  |
 *  ------------       ------------ 
*/
Node *build_queue(hist_arr freq_table) {
	if (freq_table.count == 0)
		return NULL;
	Node *root = create_new_node(freq_table.items[0]);
	Node *temp = root;
	for (int i = 1; i < freq_table.count; ++i) {
		Node* new = create_new_node(freq_table.items[i]);
		new->prev = temp;
		temp->next = new;
		temp = new;
	}
	return root;
}

void free_list(Node *root) {
	Node *current = root;
	Node *next = NULL;
	while (current) {
		next = current->next;
		free(current);
		current = next;
	}
}

void print_list(Node *root) {
	while (root) {
		Node *prev = root->prev;
		if (prev) {
			printf("current node: (%d:%zu) -- ", root->value.c, root->value.freq);
			printf("previous node: (%d:%zu)\n", prev->value.c, prev->value.freq);
		}
		printf("current node: (%d:%zu)\n", root->value.c, root->value.freq);
		root = root->next;
	}
}

#if 0
int main() {
	Node *n = create_new_node((c_freq){.c = 'h', .freq = 2});
	printf("char: %c, freq: %zu\n", n->value.c, n->value.freq);
	free(n);
}
#endif
