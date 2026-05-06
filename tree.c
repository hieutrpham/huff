#include "main.h"
#include <stdlib.h>

TreeNode *init_tree_node() {
	TreeNode *n = malloc(sizeof(*n));
	n->l = NULL;
	n->r = NULL;
	n->weight = 0;
	assert(n);
	return n;
}

TreeNode *build_huffman_tree(Queue *initial, Queue* combined) {
	if (is_empty(combined)) {
		assert(initial->len > 1);
		TreeNode *new =  init_tree_node();
		new->l = init_tree_node();
		new->l->weight = dequeue(initial)->value.freq;
		new->r = init_tree_node();
		new->r->weight = dequeue(initial)->value.freq;
		new->weight = new->l->weight + new->r->weight;
		enqueue(combined, new);
		if (is_empty(initial))
			return new;
	}
}
