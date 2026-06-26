#include "main.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Node *create_new_node(struct Arena *a, c_freq v) {
	Node *new_node = arena_malloc(a, sizeof(*new_node));
	new_node->value = v;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// linked list structure:
/*
 *  ------------       ------------
 * |value: |c   |     |value: |c   |
 * |       |freq|  -> |       |freq|
 * |Node* next  |     |Node* next  |
 * |Node* prev  |     |Node* prev  |
 *  ------------       ------------
 *
 * Queue structure:
 * Node *back -> this is the root of the linked list
 * Node *front -> this is the end of the linked list
 *
*/

// this function builds the first queue to be used in Huffman algo.
// it takes an already sorted frequency table
Queue *build_queue_from_table(struct Arena *a, hist_arr *freq_table) {
	if (freq_table->count == 0)
		return NULL;

	Queue *q = init_queue(a);
	for (int i = freq_table->count - 1; i >= 0; --i) {
		Node *n = create_new_node(a, freq_table->items[i]);
		enqueue_list(q, n);
	}
	return q;
}

Queue *init_queue(struct Arena *a) {
	Queue *q = arena_malloc(a, sizeof(*q));
	q->back = NULL;
	q->front = NULL;
	q->len = 0;
	return q;
}

void enqueue_list(Queue *q, Node *n) {
	assert(n);

	if (is_empty(q)) {
		q->back = n;
		q->front = n;
		q->len++;
		return;
	}

	Node *old_back = q->back;
	n->next = old_back; // new node is back of queue
	q->back = n;
	old_back->prev = n; // back of queue prev node is new node
	if (q->len <= 1)
		q->front = old_back; // front of queue is back if queue len <= 1
	q->len++;
}

Node* dequeue_list(Queue *q) {
	assert(q);
	assert(q->len > 0);
	Node *front = q->front;
	assert(front);
	Node *back = q->back;
	assert(back);

	if (q->len == 1) {
		assert(q->front == q->back);
		Node *n = q->back;
		q->back = NULL;
		q->front = NULL;
		q->len--;
		return n;
	}

	Node *new_front = front->prev;
	q->front = new_front;
	q->len--;
	return front;
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
	if (q->len > 0) {
		free_list(q->back);
		free(q);
		return;
	}
	free(q);
	q = NULL;
}

void print_list(Node *root) {
	while (root) {
		printf("current node: (%c:%u)\n", root->value.c, root->value.freq);
		root = root->next;
	}
}

void print_queue(const Queue *queue, const char *str)
{
	Node *n = queue->back;
	if (queue->len == 0) {
		printf("Queue %s: [ so empty ]\n", str);
		fflush(stdout);
		return;
	}

	printf("Queue %s: ", str);
	while (n) {
		if (n->next)
			printf("(%c:%u) -> ", n->value.c, n->value.freq);
		else
			printf("(%c:%u)", n->value.c, n->value.freq);
		n = n->next;
	}
	printf("\n");
	fflush(stdout);
}

boolean is_empty(Queue *q) {
	return q->len == 0;
}
