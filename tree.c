#include "main.h"
#include <stdlib.h>

TreeNode *init_tree_node() {
	TreeNode *n = malloc(sizeof(*n));
	n->l = NULL;
	n->r = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->weight = 0;
	assert(n);
	return n;
}

void enqueue_tree(Queue *q, TreeNode *n) {
	assert(n);

	if (is_empty(q)) {
		q->back = n;
		q->front = n;
		q->len++;
		return;
	}

	TreeNode *old_back = q->back;
	n->next = old_back; // new node is back of queue
	q->back = n;
	old_back->prev = n; // back of queue prev node is new node
	if (q->len <= 1)
		q->front = old_back; // front of queue is back if queue len <= 1
	q->len++;
}

TreeNode* dequeue_tree(Queue *q) {
	assert(q);
	assert(q->len > 0);
	TreeNode* front = q->front;
	assert(front);
	TreeNode* back = q->back;
	assert(back);

	if (q->len == 1) {
		assert(q->front == q->back);
		TreeNode *n = q->back;
		q->back = NULL;
		q->front = NULL;
		q->len--;
		return n;
	}

	TreeNode *new_front = front->prev;
	q->front = new_front;
	new_front->next = NULL;
	q->len--;
	return front;
}

TreeNode *build_huffman_tree(Queue *initial, Queue* combined) {
	if (is_empty(combined)) {
		assert(initial->len > 1);
		TreeNode *new =  init_tree_node();
		new->l = init_tree_node();
		new->l->weight = dequeue_tree(initial)->weight;
		new->r = init_tree_node();
		new->r->weight = dequeue_tree(initial)->weight;
		new->weight = new->l->weight + new->r->weight;
		enqueue_tree(combined, new);
		if (is_empty(initial))
			return new;
	}
}
