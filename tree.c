#include "main.h"
#include <stdlib.h>

TreeNode *init_tree_node() {
	TreeNode *n = malloc(sizeof(*n));
	assert(n);
	n->l = NULL;
	n->r = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->weight = 0;
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
	while (initial->len + combined->len >= 2) {
		// NOTE: if the combined queue is empty, dequeue the 2 nodes from initial queue and add them to the combined queue
		if (is_empty(combined)) {
			Node *i_node = dequeue_node(initial);

			TreeNode *new_tree_node = init_tree_node();
			new_tree_node->l = init_tree_node();
			new_tree_node->l->weight = i_node->value.freq;

			i_node = dequeue_node(initial);

			new_tree_node->r = init_tree_node();
			new_tree_node->r->weight = i_node->value.freq;
			new_tree_node->weight = new_tree_node->l->weight + new_tree_node->r->weight;

			enqueue_tree(combined, new_tree_node);
			if (is_empty(initial))
				break;
		}
		if (is_empty(initial)) {
			assert(!is_empty(combined));
			if (combined->len == 1)
				break;
			TreeNode *new_tree_node = init_tree_node();
			TreeNode *temp = dequeue_tree(combined);
			new_tree_node->l = init_tree_node();
			new_tree_node->l->weight = temp->weight;

			temp = dequeue_tree(combined);
			new_tree_node->r = init_tree_node();
			new_tree_node->r->weight = temp->weight;

		}
	}

	assert(is_empty(initial));
	assert(combined->len + initial->len == 1);
	return dequeue_tree(combined);
}
