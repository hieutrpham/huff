#include "main.h"
#include <assert.h>

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

// linked list structure:
/*
 *  ------------       ------------ 
 * |value: |c   |     |value: |c   |
 * |       |freq|  -> |       |freq|
 * |Node* next  |     |Node* next  |
 *  ------------       ------------ 
 *
 * Queue structure:
 * Node *back -> this is the root of the linked list
 * Node *front -> this is the end of the linked list
 *
*/

// this function builds the first queue to be used in Huffman algo.
// it takes an already sorted frequency table
Queue *build_queue(hist_arr freq_table) {
	Queue *q = malloc(sizeof(*q));
	if (!q) {
		fprintf(stderr, "ERR: malloc\n");
		exit(MALLOC_ERR);
	}

	if (freq_table.count == 0)
		return NULL;
	q->back = create_new_node(freq_table.items[0]);
	Node *temp = q->back;
	for (int i = 1; i < freq_table.count; ++i) {
		Node* new = create_new_node(freq_table.items[i]);
		temp->next = new;
		temp = new;
	}
	q->front = temp;
	return q;
}

void queue(Queue *q, Node *n) {
	assert(q);
	assert(n);
	Node *back = q->back;
	n->next = back;
	q->back = n;
}

Node* dequeue(Queue *q) {
	if (!q)
		return NULL;
	Node *f = q->front;
	Node *temp = q->back;
	while (temp && temp->next != f) {
		temp = temp->next;
	}
	q->front = temp;
	return f;
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

void free_queue(Queue *q) {
	free_list(q->back);
	free(q);
}

void print_list(Node *root) {
	while (root) {
		printf("current node: (%c:%zu)\n", root->value.c, root->value.freq);
		root = root->next;
	}
}

#if 0
int main() {
}
#endif
